#pragma once
#include <SDL.h>
#include <SDL_image.h>

#include <memory>

#include "ObjectManager.h"

class Game
{
public:
	Game();
	bool init();

	void splash();
	void run();
	void quit();
private:
	void input();
	void render(int);
	void update();

	SDL_Window* window{};
	SDL_Renderer* renderer{};

	std::unique_ptr<ObjectManager> objectManager;

	bool bRun{ true };
	int Fade{ 0 };
};

