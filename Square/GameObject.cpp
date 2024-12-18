#include "GameObject.h"

GameObject::GameObject()
{
}

GameObject::GameObject(const std::string& name)
	: _objectName{ name }
{
}

GameObject::~GameObject()
{
}

bool GameObject::MakeRect(SDL_Renderer* renderer, const SDL_Rect& rect, const SDL_Color& rectColor)
{
	SDL_Surface* _surface = SDL_CreateRGBSurface
	(0, rect.w, rect.h, 32,
		rectColor.r, rectColor.g, rectColor.b, rectColor.a);
	if (!_surface)
	{
		SDL_Log("surface invalid : %s", SDL_GetError());
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

bool GameObject::LoadImage(SDL_Renderer* renderer, const std::string& str)
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
		nextAngle += 2 * (std::abs(_rotate_amount) / 360 + 1) * M_PI;
	else if (nextAngle > 2 * M_PI)
		nextAngle -= 2 * (std::abs(_rotate_amount) / 360) * M_PI;
	_angle = nextAngle;
}
