#pragma once

#include "SpriteComponent.h"
#include "ObjectEssentials.h"

#include <string>

struct TTF_Font;
class TextComponent : public SpriteComponent
{
public:
	TextComponent(std::shared_ptr<GameObject> const&, int);
	virtual ~TextComponent();

	virtual void draw(SDL_Renderer* renderer) override;
	// Text, std::unique_ptr(AssetManager)const&, std::string
	virtual bool init(const std::vector<std::any>&) override;
	void SetColor(SDL_Renderer* renderer, const SDL_Color& color);
	void SetFont(std::unique_ptr<class AssetManager>const&);
private:
	Text _text;
	TTF_Font* _font{ nullptr };
}

