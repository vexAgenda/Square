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

