#pragma once

#include "SpriteComponent.h"
#include "ObjectEssentials.h"

struct TTF_Font;
class TextComponent : public SpriteComponent
{
public:
	TextComponent(std::shared_ptr<GameObject> const&, int);
	virtual ~TextComponent();

	// Text, std::string
	virtual bool init(const std::vector<std::any>&) override;
	void SetColor(SDL_Renderer* renderer, const SDL_Color& color);
private:
	Text _text;
	TTF_Font* _font = { nullptr };
};

