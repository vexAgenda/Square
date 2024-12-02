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
	TITLE_ENTER,
	TITLE_END,
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
	void state();
	void input();
	void update();

	void render();
	void renderFadeOut(std::shared_ptr<GameObject> object);
	void renderFadeIn(std::shared_ptr<GameObject> object);

	void titleEnter();
	void title();
	// object-related
	template <typename T>
	std::shared_ptr<T> CreateObject(const std::string& objectName, const std::string&
		fileName, Vector2F* pos, MoveType mType)
	{
		auto object = std::make_shared<T>(objectName);
		objectManager->AddObject(object);
		object->LoadImage(renderer, fileName);
		object->InitMove(pos[0], pos[1], mType);
		return object;
	}
	template <typename T>
	std::shared_ptr<T> CreateMoveTargetObject(const std::string& objectName, const std::string&
		fileName, Vector2F* pos, MoveType mType)
	{
		auto object = CreateObject<T>(objectName, fileName, new Vector2F[2]{ pos[0],pos[1] }, mType);
		Vector2F targetPos = pos[2];
		object->InitMove(pos[0], pos[1], mType);
		object->PushTarget(targetPos);
		return object;
	}

	SDL_Window* window{};
	SDL_Renderer* renderer{};

	std::unique_ptr<ObjectManager> objectManager;
	std::unique_ptr<EventHandler> eventHandler;

	bool bRun{ true };
	int Fade{ 0 };
	Uint32 curTick{};
	GameState gameState{ GameState::START };
	Vector2 _mouse{};

	const int scrX{ 512 };
	const int scrY{ 768 };

	bool stageSelectCalled{ false };
};

