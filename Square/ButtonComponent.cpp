#include "ButtonComponent.h"
#include <SDL/SDL.h>
#include "EventHandler.h"
#include "GameObject.h"
#include "ObjectEssentials.h"

ButtonComponent::ButtonComponent(std::shared_ptr<GameObject> const& owner, int order)
	: Component{owner,order}
{
}

ButtonComponent::~ButtonComponent()
{
	if (!_bind)
		SDL_Log("_bind removed!");
}

void ButtonComponent::BindEvent(Event& e)
{
	_bind = std::move(std::make_unique<Event>(e));
}

void ButtonComponent::ClickEvent(std::unique_ptr<EventHandler>& eventHandler)
{
	if (_bind)
	{
		eventHandler->PushEvent(std::make_shared<Event>(*_bind));
	}
}

bool ButtonComponent::is_hover(const Vector2& mouse)
{
	return mouse.x >= _owner->hitbox().x &&
		mouse.x <= _owner->hitbox().x + _owner->hitbox().w &&
		mouse.y >= _owner->hitbox().y &&
		mouse.y <= _owner->hitbox().y + _owner->hitbox().h;
}
