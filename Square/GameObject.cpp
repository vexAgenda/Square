
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
	_posRect = _imageRect;
	SDL_Log("Loaded Image from %s", str.c_str());
	_texture = SDL_CreateTextureFromSurface(renderer, _surface);
	if (_texture == NULL)
	{
		SDL_Log("Failed to Create Texture : %s", SDL_GetError());

	}
	SDL_FreeSurface(_surface);
	return true;
}

void GameObject::InitMove(const Pos& initPos,const Pos& velocity, const MoveType& moveType, Pos* target)
{
	SetPos(initPos.x, initPos.y);
	SetMoveVelocity(velocity);
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
		if (_posRect.x + _velocity.x >= _targetPos->x
			&& _posRect.y + _velocity.y >= _targetPos->y)
		{
			_posRect.x = _targetPos->x;
			_posRect.y = _targetPos->y;
			_velocity = { 0,0 };
		}
	}

	switch (_moveType)
	{
	case MoveType::DEFAULT:
	{
		MoveDefault(deltaTime);
		break;
	}
	case MoveType::EXPONENT:
		MoveExponential(deltaTime);
		break;
	}



}

void GameObject::SetMoveTarget(const Pos& target)
{
	if (_targetPos == nullptr)
	{
		_targetPos = new Pos{ target.x,target.y };
	}
}

void GameObject::MoveDefault(float deltaTime)
{
	_posRect.x += _velocity.x * deltaTime;
	_posRect.y += _velocity.y * deltaTime;
}

void GameObject::MoveExponential(float deltaTime)
{
	if (_velocity.x > 0)
	{
		_posRect.x += _velocity.x * _velocity.x * deltaTime;
	}
	else
	{
		_posRect.x += _velocity.x * _velocity.x * deltaTime * -1;
	}
	if (_velocity.y > 0)
	{
		_posRect.y += _velocity.y * _velocity.y * deltaTime;
	}
	else
	{
		_posRect.y += _velocity.y * _velocity.y * deltaTime * -1;
	}

}
