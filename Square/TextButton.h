#pragma once

#include "Button.h"
#include "TextLabel.h"
#include "SDL/SDL_ttf.h"
class TextButton :public Button, public TextLabel
{
public:
	TextButton(const std::string& name) : Button(), TextLabel(name) {}
	virtual ~TextButton() {}
	bool LoadImage(SDL_Renderer* renderer, const std::string&) override;
	virtual void SetColor(SDL_Renderer* renderer, const SDL_Color& color);
};
