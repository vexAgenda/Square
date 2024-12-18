#include "MoveComponent.h"
#include "GameObject.h"

MoveComponent::MoveComponent(std::shared_ptr<GameObject> const& owner, int order)
	: Component{owner,order}
{
}

MoveComponent::~MoveComponent()
{
}

bool MoveComponent::init(const std::vector<std::any>& _arg)
{
	try
	{
		SetPos(std::any_cast<Vector2F>(_arg[0]));
		SetVelocity(std::any_cast<Vector2F>(_arg[1]));
	}
	catch (std::bad_any_cast)
	{
		SDL_Log("bad any cast!");
		return false;
	}
	return true;
}

void MoveComponent::SetPos(const Vector2F& pos)
{
	if (_owner)
	{
		_owner->posRect().x = pos.x;
		_owner->posRect().y = pos.y;
	}
}

void MoveComponent::SetVelocity(const Vector2F& vel, bool hasOnlyOne)
{
	if (_velocity.empty() || !hasOnlyOne)
		_velocity.push(vel);
	else
		_velocity.front() = vel;
}

void MoveComponent::update(float deltaTime)
{
	if (hasNoTarget())
		Move(deltaTime);
	else
		MoveToTarget(deltaTime);
	Rotate(deltaTime);
}

void MoveComponent::Move(float deltaTime)
{
	if (_velocity.empty() || _canMove)
		return;
	Vector2F velocity = _velocity.front();
	_owner->posRect().x += velocity.x * deltaTime;
	_owner->posRect().y += velocity.y * deltaTime;
	_owner->hitbox().x = _owner->posRect().x;
	_owner->hitbox().y = _owner->posRect().y;
}

void MoveComponent::MoveToTarget(float deltaTime)
{
	if (!_isTargetted)
		return;
	if (_targetCoords.empty())
	{
		_isTargetted = false;
		SDL_Log("%s : Move Done!", _owner->name().c_str());
		stop();
		return;
	}
	Vector2F target = _targetCoords.front();
	Vector2F velocity = _velocity.front();
	Move(deltaTime);
	if ((velocity.x < 0 && _owner->posRect().x <= target.x) ||
		(velocity.x > 0 && _owner->posRect().x >= target.x))
	{
		_owner->posRect().x = target.x;
	}
	if ((velocity.y < 0 && _owner->posRect().y <= target.y) ||
		(velocity.y > 0 && _owner->posRect().y >= target.y))
	{
		_owner->posRect().y = target.y;
	}
	if (_owner->posRect().x == target.x && _owner->posRect().y == target.y)
	{
		_targetCoords.pop();
		if (_velocity.size() > 1)
			_velocity.pop();
		else
			_velocity.front() = { 0,0 };
	}
}

void MoveComponent::Rotate(float deltaTime)
{
	double nextAngle = _owner->angle() + _rotateAmount / 180.f * M_PI;
	if (nextAngle < 0)
		nextAngle += 2 * (std::abs(_rotateAmount) / 360 + 1) * M_PI;
	else if (nextAngle > 2 * M_PI)
		nextAngle -= 2 * (std::abs(_rotateAmount) / 360) * M_PI;
	_owner->angle() = nextAngle * deltaTime;
}

void MoveComponent::PushTarget(const Vector2F& pos)
{
	go();
}
