#include "SpriteComponent.h"
#include "GameObject.h"

#include <SDL/SDL_image.h>
#include <SDL/SDL.h>

SpriteComponent::SpriteComponent(std::shared_ptr<GameObject> const& owner, int order)
	: Component::Component(owner,order)
{
}

SpriteComponent::~SpriteComponent()
{
	SDL_DestroyTexture(_texture);
	SDL_DestroyTexture(_background);
}

bool SpriteComponent::init(const std::vector<std::any>& _args)
{
	try 
	{
		SetFadeType(std::any_cast<Fade>(_args[0]));
		SetCurrentFade(std::any_cast<int>(_args[1]));
		SetFadeAmount(std::any_cast<int>(_args[2]));
	}
	catch (std::bad_any_cast)
	{

	}
}

void SpriteComponent::draw(SDL_Renderer* renderer)
{
	if (_visible)
	{
		SDL_FRect posRect = _owner->posRect();
		/*SDL_QueryTexture(_texture, NULL, NULL, nullptr,
			nullptr);*/
		SDL_SetTextureBlendMode(_texture, SDL_BLENDMODE_BLEND);
		drawTextureFade();

		if (SDL_RenderCopyExF(renderer, _texture,
			nullptr, &posRect, _owner->angle(), NULL, SDL_FLIP_NONE)
			!= 0)
		{
			SDL_Log("Render Failed: %s", SDL_GetError());
		}
	}
}

void SpriteComponent::SetTexture(SDL_Texture* texture)
{
	_texture = texture;
	int w, h;
	SDL_QueryTexture(_texture, nullptr, nullptr, &w, &h);
	_owner->posRect().w = static_cast<float>(w);
	_owner->posRect().h = static_cast<float>(h);
}

void SpriteComponent::SetBackground(SDL_Renderer* renderer,SDL_Color rectColor)
{

	SDL_Surface* _surface = SDL_CreateRGBSurface
	(0, _owner->posRect().w, _owner->posRect().h, 32,
		rectColor.r, rectColor.g, rectColor.b, rectColor.a);
	if (!_surface)
	{
		SDL_Log("surface invalid : %s", SDL_GetError());
	}
	_rectColor = rectColor;
	_texture = SDL_CreateTextureFromSurface(renderer, _surface);
	if (_texture == NULL)
	{
		SDL_Log("Failed to Create Texture : %s", SDL_GetError());
	}
	SDL_FreeSurface(_surface);
}

void SpriteComponent::drawTextureFade()
{
	if (_fadeAmount * _currentFade > 255)
		_currentFade = 255 / _fadeAmount;
	if (_currentFade > 0)
	{
		switch (_fadeType)
		{
		case Fade::IN:
			SDL_SetTextureAlphaMod(_texture, 255 - _fadeAmount * _currentFade);
			break;
		case Fade::OUT:
			SDL_SetTextureAlphaMod(_texture, _fadeAmount * _currentFade);
		}
		_currentFade -= _fadeAmount;
	}
}