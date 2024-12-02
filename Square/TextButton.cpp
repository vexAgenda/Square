#include "TextButton.h"
#include "SDL/SDL_ttf.h"

void TextButton::PreloadInit(const Text& text)
{
	_text._ptSize = text._ptSize;
	_text._fontColor = text._fontColor;
	_text._msg = text._msg;
}

bool TextButton::LoadImage(SDL_Renderer* renderer, const std::string& str)
{
	if (_fileName == str)
	{
		return false;
	}
	TTF_Font* font = TTF_OpenFont(str.c_str(), _text._ptSize);
	if (!font)
	{
		SDL_Log("%s does not exist!", str.c_str());
		return false;
	}
	_fileName = str;
	SDL_Surface* _surface = TTF_RenderText_Solid(font, _text._msg.c_str(), _text._fontColor);
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
	TTF_CloseFont(font);
	return true;
}
