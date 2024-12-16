#include "EventHandler.h"

EventHandler::EventHandler()
{
}

EventHandler::~EventHandler()
{
}

void EventHandler::PushEvent(std::shared_ptr<Event> event)
{
	_events.push(event);
}

std::shared_ptr<Event> EventHandler::PopEvent()
{
	if (_events.empty())
	{
		return nullptr;
	}
	auto p = _events.front();
	_events.pop();
	return p;
}