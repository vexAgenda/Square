#pragma once
#include "Button.h"
#include "SDL/SDL_ttf.h"
class TextLabel : virtual public GameObject
{
public:
	TextLabel() = default;
	TextLabel(const std::string& name) { _objectName = name; }
	~TextLabel();
	void PreloadInit(const Text& text);
	virtual bool LoadImage(SDL_Renderer* renderer, const std::string&) override;
	void SetColor(SDL_Renderer* renderer, const SDL_Color& color);
	Text text() { return _text; }

protected:
	Text _text;
	TTF_Font* _font = { nullptr };
};
