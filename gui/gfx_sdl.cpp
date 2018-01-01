#include <iostream>

#include "gfx_sdl.hpp"

#include "events/message_resize.hpp"
#include "events/message_framerate.hpp"

#include "jds_counter.hpp"
#include "jds_font_sdl.hpp"
#include "jds_gl_glew.h"
#include "jds_shader.hpp"
#include "jds_eigen_opengl.hpp"
#include "jds_obj_model.h"
#include "jds_vertex_buffer_object.hpp"

#include <Eigen/Core>
#include <Eigen/Geometry>

#ifdef _MSC_VER
#pragma warning(disable : 4996) // crt is secure, disable the warning
#define snprintf _snprintf_s
#endif

//------------------------------------------------------------------------------
gfx_sdl::gfx_sdl(events::manager_interface *emi)
{
	this->emi = emi;
	
	frames = 0;
	framerate = 0;
	fps_time = 0.0f;

	rot_vel = 16.0f;
	
	fps_counter = new jds::counter();
	update_counter = new jds::counter();
	s = NULL;
	mesh = NULL;
}

//------------------------------------------------------------------------------
gfx_sdl::~gfx_sdl()
{
	// this is freed somewhere else (main)
	emi = NULL;
	
	if(update_counter)
		delete update_counter;
	if(fps_counter)
		delete fps_counter;
	if(s)
		delete s;
	if(fast_vert_vbo)
		delete fast_vert_vbo;
	if(fast_norm_vbo)
		delete fast_norm_vbo;
	if(slow_vert_vbo)
		delete slow_vert_vbo;
	if(slow_norm_vbo)
		delete slow_norm_vbo;
	if(mesh)
	{
		jds_obj_model_free(mesh);
		free(mesh);
	}

	deinit_sdl();
}

//------------------------------------------------------------------------------
void gfx_sdl::resize(int w, int h)
{
	win_w = w;
	win_h = h;
	
	glViewport(0, 0, win_w, win_h);
	jds::perspective(65.0f, (float)win_w / (float)win_h, 0.01f, 40.0f, P);
	//jds::ortho(0.0f, (float)win_w, 0.0f, (float)win_h, -10.0f, 10.0f, P);
	
	MVP = P * MV;

	if(s)
	{
		s->use();
		s->set_uniform("MVP", MVP);
	}
}

//------------------------------------------------------------------------------
void gfx_sdl::init_gl(int w, int h)
{
	// GL init
	jds_start_gl();
	print_opengl_error();
	print_opengl_info();
	
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND); // enable alpha channel
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glEnable(GL_POLYGON_SMOOTH);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	
	// HACK: to get around GLES 2.0 not supporting VAOs and OpenGL 3.2 core
	// requiring one to be bound to use VBOs
#ifndef USING_GLES2
	glGenVertexArrays(1, &default_vao);
	glBindVertexArray(default_vao);
#endif

	trans = {0.0f, 0.0f, 0.0f};
	rot = {0.0f, 0.0f, 0.0f};

	eye = Eigen::Vector3f(0.0f, 0.0f, 3.0f);
	target = Eigen::Vector3f(0.0f, 0.0f, 0.0f);
	up = Eigen::Vector3f(0.0f, 1.0f, 0.0f);

	jds::look_at(eye, target, up, V);
	
	// initialize some defaults
	color = Eigen::Vector4f(1.0f, 1.0f, 1.0f, 1.0f);
	La = Eigen::Vector3f(1.0f,1.0f, 1.0f);
	Ls = Eigen::Vector3f(1.0f,1.0f, 1.0f);
	Ld = Eigen::Vector3f(1.0f,1.0f, 1.0f);
	rot = Eigen::Vector3f(0.0f, 0.0f, 0.0f);
	trans = Eigen::Vector3f(0.0f, 0.0f, 0.0f);
	scale = 1.0f;
	light_pos = Eigen::Vector4f(eye[0], eye[1], eye[2], 1.0f);
	M = Eigen::Matrix4f::Identity();
	jds::perspective(65.0f, (float)w / (float)h, 0.01f, 40.0f, P);
	
	MV = V * M;

	normal_matrix(0, 0) = MV(0, 0);
	normal_matrix(0, 1) = MV(0, 1);
	normal_matrix(0, 2) = MV(0, 2);

	normal_matrix(1, 0) = MV(1, 0);
	normal_matrix(1, 1) = MV(1, 1);
	normal_matrix(1, 2) = MV(1, 2);

	normal_matrix(2, 0) = MV(2, 0);
	normal_matrix(2, 1) = MV(2, 1);
	normal_matrix(2, 2) = MV(2, 2);

	MVP = P * MV;
	
	print_opengl_error();
	
	char vert_file[256], frag_file[256];
	snprintf(vert_file, 256, "%s/shaders/smooth_ads_twoside_v150.vert", data_root.c_str());
	snprintf(frag_file, 256, "%s/shaders/smooth_ads_twoside_v150.frag", data_root.c_str());

	s = new jds::shader();
	s->name = "smooth_ads_v150";
	s->load_pair(vert_file, frag_file);

	s->use();
	s->vertex = glGetAttribLocation(s->id, "vertex");
	s->normal = glGetAttribLocation(s->id, "normal");
	
	s->set_uniform("light_pos", light_pos);
	s->set_uniform("La", La);
	s->set_uniform("Ls", Ls);
	s->set_uniform("Ld", Ld);
	
	s->set_uniform("color", color);

	s->set_uniform("MVP", MVP);
	s->set_uniform("MV", MV);
	s->set_uniform("normal_matrix", normal_matrix);

	print_opengl_error();

	char mesh_file[128];
	// blade1, bunny, dragon3, icosphere2
	snprintf(mesh_file, 128, "%s/meshes/dragon3.obj", data_root.c_str());
	//m = new jds::mesh_obj();
	//m->load(mesh_file);
	mesh = (JDS_OBJ_MODEL *)malloc(sizeof(JDS_OBJ_MODEL));
	jds_obj_model_load(mesh_file, mesh);
	if(jds_obj_model_check_arrays(mesh))
		jds_obj_model_gl_arrays(mesh);
	jds_obj_model_print_info(mesh);
	printf("Model has %d verticies\n", mesh->vert_count);
	printf("Vertex data takes up %.3f MB\n", (float)mesh->vert_count * 12 /
		(1024 * 1024));
	printf("Normal data takes up %.3f MB\n", (float)mesh->vert_count * 12 /
		(1024 * 1024));
		
	// see if we have a material file to load uniforms from
	if(mesh->mtl)
	{
		Ka = Eigen::Vector3f(mesh->mtl->Ka[0], mesh->mtl->Ka[1], mesh->mtl->Ka[2]);
		Ks = Eigen::Vector3f(mesh->mtl->Ks[0], mesh->mtl->Ks[1], mesh->mtl->Ks[2]);
		Kd = Eigen::Vector3f(mesh->mtl->Kd[0], mesh->mtl->Kd[1], mesh->mtl->Kd[2]);
		// TODO: make sure Ns is what we want for shininess
		shininess = mesh->mtl->Ns;

		//this->texture_filename = m->mtl->texture_filename;
	}
	// otherwise create some defaults at least
	else
	{
		Ka = Eigen::Vector3f(0.3f, 0.3f, 0.3f);
		Ks = Eigen::Vector3f(0.1f, 0.1f, 0.1f);
		Kd = Eigen::Vector3f(0.6f, 0.6f, 0.6f);
		shininess = 5.0f;
	}
	
	s->set_uniform("shininess", shininess);
	s->set_uniform("Ka", Ka);
	s->set_uniform("Ks", Ks);
	s->set_uniform("Kd", Kd);
	
	print_opengl_error();
	
	fast_vert_vbo = new jds::vertex_buffer_object();
	fast_vert_vbo->init(1, 3, mesh->vert_count, GL_ARRAY_BUFFER, GL_STATIC_DRAW,
		mesh->v);
	fast_norm_vbo = new jds::vertex_buffer_object();
	fast_norm_vbo->init(3, 3, mesh->vert_count, GL_ARRAY_BUFFER, GL_STATIC_DRAW,
		mesh->vn);
	slow_vert_vbo = new jds::vertex_buffer_object();
	slow_vert_vbo->init(1, 3, mesh->vert_count, GL_ARRAY_BUFFER, GL_STREAM_DRAW,
		mesh->v);
	slow_norm_vbo = new jds::vertex_buffer_object();
	slow_norm_vbo->init(3, 3, mesh->vert_count, GL_ARRAY_BUFFER, GL_STREAM_DRAW,
		mesh->vn);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	

	print_opengl_error();

	fflush(stdout);
}

//------------------------------------------------------------------------------
void gfx_sdl::init(int w, int h, const std::string &data_root)
{
	win_w = w;
	win_h = h;
	this->data_root = data_root;
	
	// start SDL and create the window
	init_sdl("tbsg2");
	
	print_sdl_version();
	

	this->resize(win_w, win_h);

	emi->subscribe("render_graphics", (events::observer *)this);
}

//------------------------------------------------------------------------------
void gfx_sdl::render()
{
	int status = SDL_GL_MakeCurrent(window, context);
	if(status)
	{
		printf("SDL_GL_MakeCurrent() failed in render(): %s\n",
			SDL_GetError());
		exit(1);
		return;
	}
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	float dt = update_counter->update();
	rot[1] = rot_vel * dt;
	M = M * Eigen::AngleAxisf(rot[0] / 180.0f * (float)M_PI,
		Eigen::Vector3f::UnitX());
	M = M * Eigen::AngleAxisf(rot[1] / 180.0f * (float)M_PI,
		Eigen::Vector3f::UnitY());
	M = M * Eigen::AngleAxisf(rot[2] / 180.0f * (float)M_PI,
		Eigen::Vector3f::UnitZ());

	MV = V * M;

	normal_matrix(0, 0) = MV(0, 0);
	normal_matrix(0, 1) = MV(0, 1);
	normal_matrix(0, 2) = MV(0, 2);

	normal_matrix(1, 0) = MV(1, 0);
	normal_matrix(1, 1) = MV(1, 1);
	normal_matrix(1, 2) = MV(1, 2);

	normal_matrix(2, 0) = MV(2, 0);
	normal_matrix(2, 1) = MV(2, 1);
	normal_matrix(2, 2) = MV(2, 2);

	MVP = P * MV;

	s->use();

	s->set_uniform("MVP", MVP);
	s->set_uniform("MV", MV);
	s->set_uniform("normal_matrix", normal_matrix);

	//fast_vert_vbo->use();
	slow_vert_vbo->use();
	glEnableVertexAttribArray(s->vertex);
	glVertexAttribPointer(s->vertex, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//fast_norm_vbo->use();
	slow_norm_vbo->use();
	glEnableVertexAttribArray(s->normal);
	glVertexAttribPointer(s->normal, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawArrays(GL_TRIANGLES, 0, mesh->vert_count);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	SDL_GL_SwapWindow(window);

	update_fps();
}

//------------------------------------------------------------------------------
void gfx_sdl::process_messages(events::message_base *e)
{
	if(std::string("render_graphics").compare(e->name) == 0)
	{
		render();
	}
	else if(std::string("window_resize").compare(e->name) == 0)
	{
		events::message_resize *e2 = (events::message_resize *)e;
		resize(e2->w, e2->h);
		e2 = NULL;
	}
}

//------------------------------------------------------------------------------
void gfx_sdl::init_sdl(const std::string &window_name)
{
	int ret;
	std::string window_title = window_name.c_str();

	ret = SDL_InitSubSystem(SDL_INIT_VIDEO);
	if(ret < 0)
	{
		printf("Unable to init SDL Video: %s\n", SDL_GetError());
		exit(1);
	}

	// WARNING: OpenGL was disabled since this code uses the SDL renderer
#ifdef __ANDROID__
	window = SDL_CreateWindow(window_title.c_str(), SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED, win_w, win_h,
		SDL_WINDOW_RESIZABLE | SDL_WINDOW_BORDERLESS);
#elif __APPLE__
#include "TargetConditionals.h"
#if TARGET_OS_IPHONE
	window = SDL_CreateWindow(window_title.c_str(), SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED, win_w, win_h,
		SDL_WINDOW_RESIZABLE | SDL_WINDOW_BORDERLESS);
	#elif TARGET_IPHONE_SIMULATOR
		// iOS Simulator
	#elif TARGET_OS_MAC
	// Other kinds of Mac OS
	window = SDL_CreateWindow(window_title.c_str(), SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED, win_w, win_h,
			SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
#else
		// Unsupported platform
	#endif
#else
	window = SDL_CreateWindow(window_title.c_str(), SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED, win_w, win_h,
			SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
#endif

	if(!window)
	{
		printf("Couldn't create window: %s\n", SDL_GetError());
		exit(-1);
	}
	else
	{
		screen = SDL_GetWindowSurface(window);
		renderer = SDL_GetRenderer(window);
		if(!renderer) // not created yet?
		{
			renderer = SDL_CreateRenderer(window, -1, 0);
			if(!renderer)
			{
				std::cerr << "Failed to create renderer!\n";
				std::cerr << SDL_GetError() << "\n";
				exit(-1);
			}
		}
	}

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_ShowWindow(window);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_RETAINED_BACKING, 1);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
			SDL_GL_CONTEXT_PROFILE_CORE);

	context = SDL_GL_CreateContext(window);

	ret = SDL_GL_MakeCurrent(window, context);
	if(ret)
	{
		printf("ERROR could not make GL context current after init!\n");
		exit(1);
	}

	SDL_GL_SetSwapInterval(0);

	init_gl(win_w, win_h);

	print_opengl_error();
	
	// linear texture interpolation
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
}

//------------------------------------------------------------------------------
void gfx_sdl::deinit_sdl()
{
	if(renderer)
		SDL_DestroyRenderer(renderer);
	if(window)
		SDL_DestroyWindow(window);
	
	SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

//------------------------------------------------------------------------------
void gfx_sdl::update_fps()
{
	frames++;
	fps_time += fps_counter->update();

	if(fps_time > 1.0f)
	{
		framerate = (unsigned int)((float)frames / fps_time);
		fps_time = 0.0f;
		frames = 0;

		printf("FPS: %d\n", framerate);
		fflush(stdout);

		emi->post_event(new events::message_framerate(framerate));
	}
}

//------------------------------------------------------------------------------
void gfx_sdl::print_sdl_version()
{
	SDL_version compiled;
	SDL_version linked;


	SDL_VERSION(&compiled);
	SDL_GetVersion(&linked);
	printf("SDL compiled version %d.%d.%d\n",
		compiled.major, compiled.minor, compiled.patch);
	printf("SDL linked version %d.%d.%d\n",
		linked.major, linked.minor, linked.patch);
	printf("SDL revision number: %d\n", SDL_GetRevisionNumber());
}
