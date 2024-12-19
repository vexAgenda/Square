#include "Component.h"
#include "GameObject.h"
#include "SDL/SDL.h"

Component::Component(std::shared_ptr<class GameObject> const& owner, int order)
	: _owner{owner}, _order{order}
{
	SDL_Log("component added! : %s",_owner->name().c_str());
}

Component::~Component()
{}

bool Component::init(const std::vector<std::any>&)
{
	return true;
}

void Component::update(float deltaTime)
{}
