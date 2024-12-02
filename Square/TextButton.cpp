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
	_font = TTF_OpenFont(str.c_str(), _text._ptSize);
	if (!_font)
	{
		SDL_Log("%s does not exist!", str.c_str());
		return false;
	}
	_fileName = str;
	SDL_Surface* _surface = TTF_RenderText_Solid(_font, _text._msg.c_str(), _text._fontColor);
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

void TextButton::SetColor(SDL_Renderer* renderer ,const SDL_Color& color)
{
	SDL_DestroyTexture(_texture);
	_text._fontColor = color;
	SDL_Surface* _surface = TTF_RenderText_Solid(_font, _text._msg.c_str(), _text._fontColor);
	_imageRect = _surface->clip_rect;
	_posRect.w = _surface->clip_rect.w;
	_posRect.h = _surface->clip_rect.h;
	_hitbox = _posRect;
	_texture = SDL_CreateTextureFromSurface(renderer, _surface);
	if (_texture == NULL)
	{
		SDL_Log("Failed to Create Texture : %s", SDL_GetError());

	}
	SDL_FreeSurface(_surface);
}
