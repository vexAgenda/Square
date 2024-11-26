#pragma once
#include <SDL/SDL.h>

#include <SDL/SDL_image.h>
#include <string>

struct Pos
{
	int x;
	int y;
};
class GameObject
{
public:
	GameObject();
	virtual ~GameObject();

	bool LoadImage(SDL_Renderer* renderer,const std::string& str);

	void setPos(int x, int y)
	{
		_posRect.x = x;
		_posRect.y = y;
	}

	void Move(int x, int y)
	{
		_posRect.x += _dir.x * x;
		_posRect.y += _dir.y * y;
	}

	SDL_Texture* texture() { return _texture;  }
	SDL_Rect imageRect() { return _imageRect; }
	SDL_Rect posRect() { return _posRect; }
	std::string textureName() { return _textureName; }

private:
	SDL_Texture* _texture{ nullptr };
	SDL_Rect _imageRect{};
	SDL_Rect _posRect{};
	std::string _textureName{};
	Pos _dir;
};
