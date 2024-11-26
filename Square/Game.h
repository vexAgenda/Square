#pragma once
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include <memory>
#include <cmath>

#include "ObjectManager.h"
#include "EventHandler.h"

enum class GameState
{
	START,
	SPLASH,
	NEUTRAL,
	TITLE,
	END
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
	void title();
	void update();
	// object-related

	SDL_Window* window{};
	SDL_Renderer* renderer{};

	std::unique_ptr<ObjectManager> objectManager;
	std::unique_ptr<EventHandler> eventHandler;

	bool bRun{ true };
	int Fade{ 0 };
	Uint32 curTick{};
	GameState gameState;
};

