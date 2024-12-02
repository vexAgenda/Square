#pragma once

#include "Button.h"
#include "SDL/SDL_ttf.h"
struct Text
{
	int _ptSize{ 0 };
	SDL_Color _fontColor{};
	std::string _msg{};
};
class TextButton :public Button
{
public:
	TextButton(const std::string& name) { _objectName = name; }
	~TextButton() { TTF_CloseFont(_font); }
	void PreloadInit(const Text& text);
	bool LoadImage(SDL_Renderer* renderer,const std::string&) override;
	void SetColor(SDL_Renderer* renderer ,const SDL_Color& color);
	Text text() { return _text; }

private:
	Text _text;
	TTF_Font* _font;
};

