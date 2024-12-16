#include "TextButton.h"
#include "SDL/SDL_ttf.h"

bool TextButton::LoadImage(SDL_Renderer* renderer, const std::string& name)
{
	return TextLabel::LoadImage(renderer, name);
}

void TextButton::SetColor(SDL_Renderer* renderer, const SDL_Color& color)
{
	TextLabel::SetColor(renderer, color);
}