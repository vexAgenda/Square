#pragma once
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include <memory>
#include <cmath>
#include <unordered_map>

#include "ObjectFactory.h"
#include "ObjectManager.h"
#include "EventHandler.h"

#include "TextButton.h"

enum class GameState
{
	START,
	SPLASH,
	NEUTRAL,
	TITLE,
	TITLE_ENTER,
	TITLE_END,
	RUN_ENTER,
	RUN,
	RUN_END,
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
	//Event related Function
	void QueueEvent(EID e);
	void BindEvent(std::shared_ptr<Button>const&b, EID e);
	void MenuSelect();

	void state();
	void input();
	void update();

	void render();
	void renderTextureFadeOut(std::shared_ptr<GameObject> object);
	void renderTextureFadeIn(std::shared_ptr<GameObject> object);

	void titleEnter();
	void title();
	void interactButton(std::shared_ptr<Button> const& button);
	void titleEnd();

private:
	SDL_Window* window{};
	SDL_Renderer* renderer{};

	std::unique_ptr<ObjectManager> objectManager;
	std::unique_ptr<EventHandler> eventHandler;
	std::unique_ptr<ObjectFactory> objectFactory;

	bool bRun{ true };
	int Fade{ 0 };
	Uint32 curTick{};
	GameState gameState{ GameState::START };
	Vector2 _mouse{};

	const int scrX{ 512 };
	const int scrY{ 768 };

	bool stageSelectCalled{ false };
	std::unordered_map<std::string, bool> buttonActive;
	std::vector<bool> stageCleared = {
		true,false,false,false,false,false,
		false,false,false,false,false,false,
		false,false,false,false,false,false,
		false,false,false,false,false,false,
		false,false,false,false,false,false,
	};

	const double rad_MAX = 2 * M_PI;
	const double rad_MIN = 0;
	int mapIndex = 0;
};

