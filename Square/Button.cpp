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

void Button::is_clicked(std::unique_ptr<EventHandler> eventHandler,
	const Vector2& mouse)
{
	if (GameObject::isMouseCollide(mouse))
	{
		eventHandler->PushEvent(std::make_shared<Event>(*_bind));
		delete _bind;
		_bind = nullptr;
	}
}

void Button::BindEvent(Event& e)
{
	if (_bind)
		delete _bind;
	_bind = &e;
}
