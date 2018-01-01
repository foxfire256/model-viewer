#include "main_observer.hpp"

//------------------------------------------------------------------------------
main_observer::main_observer(events::manager_interface *emi, int *done)
{
	this->done = done;
	this->emi = emi;

	emi->subscribe("quit_application", (events::observer *)this);
}

//------------------------------------------------------------------------------
void main_observer::process_messages(events::message_base *e)
{
	*done = 1;
}
