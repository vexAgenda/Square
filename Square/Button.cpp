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
	if (!_bind)
		SDL_Log("_bind removed!");
}

void Button::BindEvent(Event& e)
{
	_bind = std::move(std::make_unique<Event>(e));
}

void Button::ClickEvent(std::unique_ptr<class EventHandler>& eventHandler)
{
	if (_bind)
	{
		eventHandler->PushEvent(std::make_shared<Event>(*_bind));
	}
}
