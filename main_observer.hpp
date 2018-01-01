#ifndef MAIN_OBSERVER_HPP
#define MAIN_OBSERVER_HPP

#include "events/observer.hpp"

/*!
 * \brief Observer class for main, handles done message
 */
class main_observer : public events::observer
{
public:
	main_observer(events::manager_interface *emi, int *done);
	virtual ~main_observer(){}

	void process_messages(events::message_base *e);

private:
	int *done;
};

#endif // MAIN_OBSERVER_HPP
