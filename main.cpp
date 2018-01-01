#include <iostream>

#include <boost/version.hpp>

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/tokenizer.hpp>
#include <boost/token_functions.hpp>

namespace po = boost::program_options;

#include <SDL2/SDL.h>

#include "events/manager.hpp"
#include "main_observer.hpp"
#include "events/console_writer.hpp"
#include "sdl_message_handler.hpp"
#include "jds_eigen_info.hpp"
#include "events/manager.hpp"
#include "gui/gfx_sdl.hpp"

int done;
int win_w, win_h;

events::manager *em;
main_observer *mo;
events::console_writer *ecw;
gfx_sdl *g;
sdl_message_handler *smh;

#ifdef __ANDROID__
std::string data_root = "/sdcard/model-viewer-data";
#elif __APPLE__
std::string data_root = "/Users/foxfire/dev/model-viewer-data";
#elif _WIN32
std::string data_root = "C:/dev/model-viewer-data";
#else // Linux
std::string data_root = "/home/foxfire/dev/model-viewer-data";
#endif

int main(int argc, char **argv)
{
	// set some defaults
	win_w = 640;
	win_h = 480;
	g = NULL;
	smh = NULL;
	
	std::cout << "Using Boost "     
		// major version
		<< BOOST_VERSION / 100000
		<< "."
		// minor version
		<< BOOST_VERSION / 100 % 1000
		<< "."
		// patch level
		<< BOOST_VERSION % 100
		<< std::endl;
	
	// parse command line
	po::options_description desc("Command line options");
	po::variables_map vm;
	
	desc.add_options()
		("help,h", "produce help message")
		("width", po::value<int>(), "set window width")
		("height", po::value<int>(), "set window height")
		("data-root", po::value<std::string>(), "set data directory")
	;
	
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);
	
	if(vm.count("help"))
	{
		std::cout << desc << "\n";
		return 1;
	}
	if(vm.count("width"))
	{
		win_w = vm["width"].as<int>();
	}
	if(vm.count("height"))
	{
		win_h = vm["height"].as<int>();
	}
	if(vm.count("data-root"))
	{
		data_root = vm["data-root"].as<std::string>();
	}

	jds::print_eigen_version();
	
	// create an events manager first
	em = new events::manager();
	
	// write info to the command line
	ecw = new events::console_writer(em);
	
	// get the observer going for main
	done = 0;
	mo = new main_observer(em, &done);
	
	// start SDL
	SDL_Init(0);
	
	g = new gfx_sdl(em);
	g->init(win_w, win_h, data_root);
	smh = new sdl_message_handler(em);
	
	fflush(stdout);
	
	// main loop
	while(!done)
	{
		smh->pump_messages();
		
		em->pump_events();
		
		g->render();
	}
	
	if(smh)
		delete smh;
	if(g)
		delete g;
	if(mo)
		delete mo;
	if(ecw)
		delete ecw;
	if(em)
		delete em;

	SDL_Quit();
	
	return 0;
	
	return 0;
}
