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

bool Button::is_hover(const Vector2& mouse)
{
	return mouse.x >= _hitbox.x && 
		mouse.x <= _hitbox.x +_hitbox.w && 
		mouse.y >= _hitbox.y && 
		mouse.y <= _hitbox.y +_hitbox.h;
}

void Button::BindEvent(Event& e)
{
	if (_bind)
		delete _bind;
	_bind = &e;
}
