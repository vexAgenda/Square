#pragma once
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_events.h>

#include <memory>

#include "ObjectManager.h"

enum EID_TYPE
{
	EID_GAME = 1
};
enum EID_CODE
{
	EID_CODE_START,
	EID_CODE_SPLASH = 0b1,
	EID_CODE_TITLE = 0b10,
	EID_CODE_END = 0b1000000
};

class Game
{
public:
	Game();
	bool init();

	void splash();
	void run();
	void quit();
private:
	void event();
	void input();
	void render(int = 0);
	void update();

	SDL_Window* window{};
	SDL_Renderer* renderer{};

	std::unique_ptr<ObjectManager> objectManager;

	bool bRun{ true };
	int Fade{ 0 };
};

