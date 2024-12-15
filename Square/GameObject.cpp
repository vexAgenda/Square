
#include "GameObject.h"


GameObject::GameObject()
{
	_velocity.push({0, 0});
}

GameObject::GameObject(const std::string& name)
: _objectName{name}
{

}

GameObject::~GameObject()
{
	SDL_DestroyTexture(_texture);
}

bool GameObject::MakeRect(SDL_Renderer* renderer, const SDL_Rect& rect , const SDL_Color& rectColor)
{
	SDL_Surface* _surface = SDL_CreateRGBSurface
	(0, rect.w, rect.h, 32,
		rectColor.r, rectColor.g, rectColor.b, rectColor.a);
	if (!_surface)
	{
		SDL_Log("surface invalid : %s" ,SDL_GetError());
	}
	_rectColor = rectColor;
	_imageRect = rect;
	_posRect.w = rect.w;
	_posRect.h = rect.h;
	_hitbox = _posRect;
	_texture = SDL_CreateTextureFromSurface(renderer, _surface);
	if (_texture == NULL)
	{
		SDL_Log("Failed to Create Texture : %s", SDL_GetError());
	}
	SDL_FreeSurface(_surface);
	return true;
}

bool GameObject::LoadImage(SDL_Renderer* renderer,const std::string& str)
{
	SDL_Surface* _surface = IMG_Load(str.c_str());
	if (_fileName == str)
	{
		return false;
	}
	if (!_surface)
	{
		SDL_Log("%s does not exist!", str.c_str());
		return false;
	}
	_fileName = str;
	_imageRect = _surface->clip_rect;
	_posRect.w = _surface->clip_rect.w;
	_posRect.h = _surface->clip_rect.h;
	_hitbox = _posRect;
	SDL_Log("Loaded Image from %s", str.c_str());
	_texture = SDL_CreateTextureFromSurface(renderer, _surface);
	if (_texture == NULL)
	{
		SDL_Log("Failed to Create Texture : %s", SDL_GetError());

	}
	SDL_FreeSurface(_surface);
	return true;
}

void GameObject::InitFade(Fade fadeType, int currentFade, int amount)
{
	SetFadeType(fadeType);
	SetCurrentFade(currentFade);
	SetFadeAmount(amount);
}

void GameObject::InitMove(const Vector2F& initPos, const Vector2F& velocity, const MoveType& mType)
{
	SetPos(initPos);
	Vector2 vel;
	vel.x = velocity.x;
	vel.y = velocity.y;
	SetVelocity(vel);
	SetMoveType(mType);

}

void GameObject::SetPos(const Vector2F& pos)
{
	_posRect.x = pos.x;
	_posRect.y = pos.y;
}

void GameObject::SetVelocity(const Vector2& vel,bool isStatic)
{
	if (_velocity.empty())
		_velocity.push(vel);
 	if (isStatic)
		_velocity.front() = vel;
	else
		_velocity.push(vel);
}

void GameObject::SetMoveType(const MoveType& mType)
{
	_moveType = mType;
}

void GameObject::Move(float deltaTime)
{
	if (_isTargetted)
		return;
	switch (_moveType)
	{
	case MoveType::DEFAULT:
		MoveDefault(deltaTime);
		break;
	case MoveType::SQUARE:
		MoveExponential(deltaTime);
		break;
	}
}

void GameObject::MoveTargetted(float deltaTime)
{
	if (!_isTargetted)
		return;
	if (_targetCoords.empty())
	{
		_isTargetted = false;
		_velocity.front() = { 0,0 };
		SDL_Log("%s : Move Done!",_objectName.c_str());
		return;
	}
	Vector2F target = _targetCoords.front();
	Vector2 velocity = _velocity.front();
	switch (_moveType)
	{
	case MoveType::DEFAULT:
		MoveDefault(deltaTime);
		break;
	case MoveType::SQUARE:
		MoveExponential(deltaTime);
		break;
	}
	if ((velocity.x < 0 && _posRect.x <= target.x) ||
		(velocity.x > 0 && _posRect.x >= target.x))
	{
		_posRect.x = target.x;
	}
	if ((velocity.y < 0 && _posRect.y <= target.y) ||
		(velocity.y > 0 && _posRect.y >= target.y))
	{
		_posRect.y = target.y;
	}
	if (_posRect.x == target.x && _posRect.y == target.y)
	{
		_targetCoords.pop();
		if (_velocity.size() > 1)
			_velocity.pop();
		else
			_velocity.front() = { 0,0 };
	}
}

void GameObject::PushTarget(const Vector2F& pos)
{
	if (!_isTargetted)
		_isTargetted = true;
	_targetCoords.push(pos);
}

bool GameObject::is_hover(const Vector2& mouse)
{

	return mouse.x >= _hitbox.x &&
		mouse.x <= _hitbox.x + _hitbox.w &&
		mouse.y >= _hitbox.y &&
		mouse.y <= _hitbox.y + _hitbox.h;

}

void GameObject::Rotate()
{
	double nextAngle = _angle + _rotate_amount / 180.f * M_PI;
	if (nextAngle < 0)
		nextAngle += 2 * (std::abs(_rotate_amount) /360 + 1) * M_PI;
	else if(nextAngle > 2 * M_PI)
		nextAngle -= 2 * (std::abs(_rotate_amount) / 360) * M_PI;
	_angle = nextAngle;
}

void GameObject::MoveDefault(float deltaTime)
{
	if (_velocity.empty())
		return;
	Vector2 velocity = _velocity.front();
	_posRect.x += velocity.x * deltaTime;
	_posRect.y += velocity.y * deltaTime;
	_hitbox.x = _posRect.x;
	_hitbox.y = _posRect.y;
}

void GameObject::MoveExponential(float deltaTime)
{
	if (_velocity.empty())
		return;
	Vector2 velocity = _velocity.front();
	if (velocity.x > 0)
		_posRect.x += velocity.x * velocity.x * deltaTime;
	else
		_posRect.x += velocity.x * velocity.x * -1 *deltaTime;
	if (velocity.y > 0)
		_posRect.y += velocity.y * velocity.y * deltaTime;
	else
		_posRect.y += velocity.y * velocity.y * -1 * deltaTime;
	_hitbox.x = _posRect.x;
	_hitbox.y = _posRect.y;
}
