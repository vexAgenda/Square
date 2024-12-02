#pragma once

#include "Button.h"
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
	~TextButton() {}
	void PreloadInit(const Text& text);
	bool LoadImage(SDL_Renderer* renderer,const std::string&) override;
	void SetColor(const SDL_Color& color) { _text._fontColor = color; }
	Text text() { return _text; }

private:
	Text _text;
};

