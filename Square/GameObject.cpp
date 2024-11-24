
#include "GameObject.h"

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
	SDL_DestroyTexture(_texture);
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
