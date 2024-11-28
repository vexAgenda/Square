
#include "GameObject.h"

GameObject::GameObject()
{
}

GameObject::GameObject(const std::string& name)
: _objectName{name}
{

}

GameObject::~GameObject()
{
	SDL_DestroyTexture(_texture);
	delete _targetPos;
}

bool GameObject::LoadImage(SDL_Renderer* renderer,const std::string& str)
{
	SDL_Surface* _surface = IMG_Load(str.c_str());
	if (!_surface)
	{
		SDL_Log("%s does not exist!", str.c_str());
		return false;
	}
	_imageRect = _surface->clip_rect;
	_posRect.x = _surface->clip_rect.x;
	_posRect.y = _surface->clip_rect.y;
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

void GameObject::InitMove(const Vector2F& initPos,const Vector2F& velocity, const MoveType& moveType, Vector2F* target)
{
	SetPos(initPos.x, initPos.y);
	Vector2 vel;
	vel.x = velocity.x;
	vel.y = velocity.y;
	SetMoveVelocity(vel);
	SetMoveType(moveType);
	if (target)
	{
		SetMoveTarget(*target);
	}
}

void GameObject::Move(float deltaTime)
{
	if (_targetPos)
	{
		if (_posRect.x + (_velocity.x * deltaTime) >= _targetPos->x
			&& _posRect.y +(_velocity.y * deltaTime) >= _targetPos->y)
		{
			SDL_Log("%s : TargerMove Done! (%f,%f)", _objectName.c_str(), _posRect.x, _posRect.y);
			_posRect.x = _targetPos->x;
			_posRect.y = _targetPos->y;
			_velocity = { 0,0 };
			return;
		}
	}

	switch (_moveType)
	{
	case MoveType::DEFAULT:
	{
		MoveDefault(deltaTime);
		break;
	}
	case MoveType::SQUARE:
		MoveExponential(deltaTime);
		break;
	}



}

void GameObject::SetMoveTarget(const Vector2F& target)
{
	SDL_Log("%s's MoveTarget : (%f,%f)", _objectName.c_str(), target.x, target.y);
	if (_targetPos)
		delete _targetPos;
	_targetPos = new Vector2F{ target.x,target.y };
}

bool GameObject::isMouseCollide(const Vector2& mouse)
{
	return mouse.x >= _hitbox.x && mouse.x <= _hitbox.x + _hitbox.w &&
		mouse.y >= _hitbox.y && mouse.y <= _hitbox.y + _hitbox.h;
}

void GameObject::MoveDefault(float deltaTime)
 {
	_posRect.x += _velocity.x * deltaTime;
	_posRect.y += _velocity.y * deltaTime;
	_hitbox.x += _velocity.x * deltaTime;
	_hitbox.y += _velocity.y * deltaTime;
}

void GameObject::MoveExponential(float deltaTime)
{
	if (_velocity.x > 0)
	{
		_posRect.x += _velocity.x * _velocity.x * deltaTime;
		_hitbox.x += _velocity.x * _velocity.x * deltaTime;
	}
	else
	{
		_posRect.x += _velocity.x * _velocity.x * deltaTime * -1;
		_hitbox.x += _velocity.x * _velocity.x * deltaTime * -1;
	}
	if (_velocity.y > 0)
	{
		_posRect.y += _velocity.y * _velocity.y * deltaTime;
		_hitbox.y += _velocity.y * _velocity.y * deltaTime;
	}
	else
	{
		_posRect.y += _velocity.y * _velocity.y * deltaTime * -1;
		_hitbox.x += _velocity.x * _velocity.x * deltaTime * -1;
	}

}
