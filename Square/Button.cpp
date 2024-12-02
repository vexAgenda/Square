#include "Button.h"
#include "EventHandler.h"

Button::Button() : GameObject()
{
}

Button::Button(const std::string& str)
{
	_objectName = str;
}

Button::~Button()
{
	delete _bind;
}

void Button::BindEvent(Event* e)
{
	if (!e)
		return;
	if (_bind)
		delete _bind;
	_bind = e;
}

void Button::ClickEvent(std::unique_ptr<class EventHandler>& eventHandler)
{
	if (_bind)
	{
		eventHandler->PushEvent(std::make_shared<Event>(*_bind));
	}
}
