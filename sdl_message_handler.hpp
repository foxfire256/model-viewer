#ifndef SDL_MESSAGE_HANDLER_HPP
#define SDL_MESSAGE_HANDLER_HPP

#include "events/observer.hpp"

/**
This handles messages from the SDL event queue.
*/
class sdl_message_handler : public events::observer
{
public:
	sdl_message_handler(events::manager_interface *emi);
	virtual ~sdl_message_handler(){}
	void pump_messages();

private:
};

#endif // SDL_MESSAGE_HANDLER_HPP
