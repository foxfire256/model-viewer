#include "sdl_message_handler.hpp"
#include "events/message_resize.hpp"
#include "events/message_keydown.hpp"
#include "events/message_keyup.hpp"
#include "events/message_mousemotion.hpp"
#include "events/message_mousebutton.hpp"
#include "events/message_mousewheel.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>

//------------------------------------------------------------------------------
sdl_message_handler::sdl_message_handler(events::manager_interface *emi)
{
	this->emi = emi;
}

//------------------------------------------------------------------------------
void sdl_message_handler::pump_messages()
{
	SDL_Event event;

	// TODO: actually do something with this
	int verbose_input = 0;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_WINDOWEVENT:
			switch (event.window.event)
			{
			case SDL_WINDOWEVENT_RESIZED:
				SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
					SDL_LOG_PRIORITY_INFO,
					"Window %d resized to %dx%d",
					event.window.windowID, event.window.data1,
					event.window.data2);
				//g->resize(event.window.data1, event.window.data2);
				//g->render();
				emi->post_event(new events::message_resize(event.window.data1,
					event.window.data2));
				break;
#ifdef VERBOSE_VIDEO_EVENTS
			case SDL_WINDOWEVENT_SHOWN:
				SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
					SDL_LOG_PRIORITY_INFO,
					"Window %d shown", event.window.windowID);
				break;
			case SDL_WINDOWEVENT_HIDDEN:
				SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
					SDL_LOG_PRIORITY_INFO,
					"Window %d hidden", event.window.windowID);
				break;
			case SDL_WINDOWEVENT_EXPOSED:
				SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
					SDL_LOG_PRIORITY_INFO,
					"Window %d exposed", event.window.windowID);
				break;
			case SDL_WINDOWEVENT_MOVED:
				SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
					SDL_LOG_PRIORITY_INFO,
					"Window %d moved to %d,%d",
					event.window.windowID, event.window.data1,
					event.window.data2);
				break;
			case SDL_WINDOWEVENT_SIZE_CHANGED:
				SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
					SDL_LOG_PRIORITY_INFO,
					"Window %d size changed to %dx%d",
					event.window.windowID, event.window.data1,
					event.window.data2);
				g->resize(event.window.data1, event.window.data2);
				g->render();
				break;
			case SDL_WINDOWEVENT_MINIMIZED:
				SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
					SDL_LOG_PRIORITY_INFO,
					"Window %d minimized", event.window.windowID);
				break;
			case SDL_WINDOWEVENT_MAXIMIZED:
				SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
					SDL_LOG_PRIORITY_INFO,
					"Window %d maximized", event.window.windowID);
				break;
			case SDL_WINDOWEVENT_RESTORED:
				SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
					SDL_LOG_PRIORITY_INFO,
					"Window %d restored", event.window.windowID);
				break;
			case SDL_WINDOWEVENT_ENTER:
				SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
					SDL_LOG_PRIORITY_INFO,
					"Mouse entered window %d",
					event.window.windowID);
				break;
			case SDL_WINDOWEVENT_LEAVE:
				SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
					SDL_LOG_PRIORITY_INFO,
					"Mouse left window %d", event.window.windowID);
				break;
			case SDL_WINDOWEVENT_FOCUS_GAINED:
				SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
					SDL_LOG_PRIORITY_INFO,
					"Window %d gained keyboard focus",
					event.window.windowID);
				break;
			case SDL_WINDOWEVENT_FOCUS_LOST:
				SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
					SDL_LOG_PRIORITY_INFO,
					"Window %d lost keyboard focus",
					event.window.windowID);
#ifdef __ANDROID__
				done = 1;
				quit(0);
#endif
				break;
#endif
			case SDL_WINDOWEVENT_CLOSE:
				SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
					SDL_LOG_PRIORITY_INFO,
					"Window %d closed", event.window.windowID);
				//done = 1;
				emi->post_event(new events::message_base("quit_application"));
				break;
			default:
#ifdef VERBOSE_VIDEO_EVENTS
				SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
					SDL_LOG_PRIORITY_INFO,
					"Window %d got unknown event %d",
					event.window.windowID, event.window.event);
#endif
				break;
			}
		case SDL_QUIT:
			//SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
			//	SDL_LOG_PRIORITY_INFO, "SDL_QUIT event");
			//done = 1;
			break;
		case SDL_DOLLARGESTURE:
			if(verbose_input)
			{
				SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
					SDL_LOG_PRIORITY_INFO, "SDL_DOLLARGESTURE event");
			}
			break;
		case SDL_FINGERMOTION:
			if(verbose_input)
			{
				SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
					SDL_LOG_PRIORITY_INFO, "SDL_FINGERMOTION event");
			}
			break;
		case SDL_FINGERDOWN:
			if(verbose_input)
			{
				SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
					SDL_LOG_PRIORITY_INFO, "SDL_FINGERDOWN event");
			}
			break;
		case SDL_FINGERUP:
			if(verbose_input)
			{
				SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
					SDL_LOG_PRIORITY_INFO, "SDL_FINGERUP event");
			}
			break;
		case SDL_KEYDOWN:
			if(verbose_input)
			{
				SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
					SDL_LOG_PRIORITY_INFO, "SDL_KEYDOWN event");
			}
			if(event.key.keysym.sym == SDLK_ESCAPE)
			{
				//done = 1;
				emi->post_event(new events::message_base("quit_application"));
			}
			else
			{
				emi->post_event(new events::message_keydown(
					event.key.keysym.sym));
			}
			break;
		case SDL_KEYUP:
			if(verbose_input)
			{
				SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
					SDL_LOG_PRIORITY_INFO, "SDL_KEYUP event");
			}
			
			emi->post_event(new events::message_keyup(event.key.keysym.sym));

			break;
		case SDL_MOUSEMOTION:
			if(verbose_input)
			{
				SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
					SDL_LOG_PRIORITY_INFO, "SDL_MOUSEMOTION event");
			}
			emi->post_event(new events::message_mousemotion(event.motion));
			break;
		case SDL_MOUSEBUTTONDOWN:
			if(verbose_input)
			{
				SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
					SDL_LOG_PRIORITY_INFO, "SDL_MOUSEBUTTONDOWN event");
			}
			emi->post_event(new events::message_mousebutton(event.button));
			break;
		case SDL_MOUSEWHEEL:
			if(verbose_input)
			{
				SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
					SDL_LOG_PRIORITY_INFO, "SDL_MOUSEWHEEL event");
			}
			emi->post_event(new events::message_mousewheel(event.wheel));
			break;
		case SDL_MULTIGESTURE:
			if(verbose_input)
			{
				SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
					SDL_LOG_PRIORITY_INFO, "SDL_MULTIGESTURE event");
			}
			break;
		case SDL_SYSWMEVENT:
			if(verbose_input)
			{
				SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
					SDL_LOG_PRIORITY_INFO, "SDL_SYSWMEVENT event");
			}
			break;
			/*
		case SDL_TOUCHBUTTONDOWN:
			if(verbose_input)
			{
				SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
					SDL_LOG_PRIORITY_INFO, "SDL_TOUCHBUTTONDOWN event");
			}
			break;
			*/
		case SDL_TEXTEDITING:
			if(verbose_input)
			{
				SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
					SDL_LOG_PRIORITY_INFO, "SDL_TEXTEDITING event");
			}
			break;
		case SDL_TEXTINPUT:
			if(verbose_input)
			{
				SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
					SDL_LOG_PRIORITY_INFO, "SDL_TEXTINPUT event");
			}
			break;
		/*
		case SDL_VIDEORESIZE:
			SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
				SDL_LOG_PRIORITY_INFO, "SDL_VIDEORESIZE event");
			break;
		*/
		case SDL_JOYAXISMOTION:
			if(verbose_input)
			{
				SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
					SDL_LOG_PRIORITY_INFO, "SDL_JOYAXISMOTION event");
			}
			break;
		default:
			break;
		}
	}
}
