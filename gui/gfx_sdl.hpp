#ifndef GFX_SDL_HPP
#define GFX_SDL_HPP

#include <SDL2/SDL.h>
#include <stdint.h>
#include <Eigen/Core>
#include <Eigen/Geometry>

#include "events/observer.hpp"
#include "fox/gfx/gl_glew.h"

namespace fox
{
	class counter;
}
namespace fox::gfx
{
	class model_loader_obj;
}

namespace jds
{
	class shader;
	class vertex_buffer_object;
}
struct JDS_OBJ_MODEL;

/**
 * @brief This handles SDL intialization and window creation
 */
class gfx_sdl : public events::observer
{
public:
	gfx_sdl(events::manager_interface *emi);
	virtual ~gfx_sdl();

	virtual void init(int w, int h, const std::string &window_name);
	virtual void render();

	/// this is the message handler from events::observer
	void process_messages(events::message_base *e);

protected:
	int win_w, win_h;
	std::string data_root;
	SDL_Window *window;
	SDL_Surface *screen;
	SDL_Renderer *renderer;
	SDL_GLContext context;
	
	Eigen::Vector3f eye, target, up;
	Eigen::Affine3f V;
	Eigen::Projective3f P;
	// model matrix (specific to the model instance)
	Eigen::Projective3f MVP;
	Eigen::Affine3f M, MV;
	// TODO: should this be a pointer from the parent?
	// TODO: should this be Affine3f ?
	Eigen::Matrix3f normal_matrix;
	// more shader uniforms
	Eigen::Vector4f light_pos, color;
	Eigen::Vector3f La, Ls, Ld;
	Eigen::Vector3f rot, trans;
	float scale;
	
	jds::shader *s;
	//struct JDS_OBJ_MODEL *mesh;
	fox::gfx::model_loader_obj *obj;

	GLuint fast_vertex_vbo;
	GLuint fast_normal_vbo;

	Eigen::Vector3f Ka, Ks, Kd;
	float shininess;
	
	fox::counter *update_counter;
	
	fox::counter *fps_counter;
	double fps_time;
	int frames, framerate;
	float rot_vel;

	void update_fps();

	void print_sdl_version();
	virtual void deinit_sdl();
	virtual void init_sdl(const std::string &window_name);

	virtual void resize(int w, int h);

	void init_gl(int w, int h);
	
private:
	uint32_t default_vao;
};

#endif
