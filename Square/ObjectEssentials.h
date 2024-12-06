#pragma once

#include <SDL/SDL.h>
struct Vector2
{
	int x;
	int y;
};

struct Vector2F
{
	float x;
	float y;
};
enum class MoveType
{
	DEFAULT,
	SQUARE,
	END
};

enum class Fade
{
	NONE,
	FADE_IN,
	FADE_OUT
};

struct Color
{
	int r{};
	int g{};
	int b{};
	int a = 255;
};


struct Text
{
	int _ptSize{ 0 };
	SDL_Color _fontColor{};
	std::string _msg{};
};

