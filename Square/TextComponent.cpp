#include "TextComponent.h"
#include "GameObject.h"
#include "SDL/SDL_ttf.h"

TextComponent::TextComponent(std::shared_ptr<GameObject> const& owner, int order)
	: SpriteComponent::SpriteComponent(owner,order)
{

}
TextComponent::~TextComponent()
{
	TTF_CloseFont(_font);
}

bool TextComponent::init(const std::vector<std::any>& _args)
{
	try
	{
		_text = std::any_cast<Text>(_args[0]);
		_font = TTF_OpenFont(std::any_cast<std::string>(_args[1]).c_str(), _text._ptSize);
		if (!_font)
		{
			SDL_Log("%s does not exist!", std::any_cast<std::string>(_args[1]).c_str());
			return false;
		}
	}
	catch (std::bad_any_cast)
	{
		return false;
	}
}

void TextComponent::SetColor(SDL_Renderer* renderer, const SDL_Color& color)
{
	SDL_DestroyTexture(_texture);
	_text._fontColor = color;
	SDL_Surface* _surface = TTF_RenderText_Solid(_font, _text._msg.c_str(), _text._fontColor);
	_owner->posRect().w = _surface->clip_rect.w;
	_owner->posRect().h = _surface->clip_rect.h;
	_owner->SetHitbox(_owner->posRect());
	_texture = SDL_CreateTextureFromSurface(renderer, _surface);
	if (_texture == NULL)
	{
		SDL_Log("Failed to Create Texture : %s", SDL_GetError());
	}
	SDL_FreeSurface(_surface);
}
