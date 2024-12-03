#pragma once
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include <memory>
#include <cmath>

#include "ObjectManager.h"
#include "EventHandler.h"

class Text;

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

struct Color
{
	int r;
	int g;
	int b;
	int a = 255;
};

struct ObjectBuffer
{
	std::string _type;
	std::string _objectName;
	std::string _fileName;
	std::string _text;
	Vector2F _originPos;
	Vector2F _velocity;
	bool hasTarget{ false };
	Vector2F _targetPos{ 0,1 };
	bool isRect{ false };
	Vector2 widthHeight;
	Color rectColor;
	MoveType _mType{};
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
	void renderTextureFadeOut(std::shared_ptr<GameObject> object);
	void renderTextureFadeIn(std::shared_ptr<GameObject> object);

	void titleEnter();
	void title();
	void titleEnd();

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
		object->PushTarget(targetPos);
		return object;
	}
	template <typename T>
	std::shared_ptr<T> CreateTextObject(const std::string& objectName, const std::string&
		fileName,const Text& text, Vector2F* pos, MoveType mType)
	{

		auto object = std::make_shared<T>(objectName);
		object->PreloadInit(text);
		objectManager->AddObject(object);
		object->LoadImage(renderer, fileName);
		object->InitMove(pos[0], pos[1], mType);
		return object;
	}

	template <typename T>
	std::shared_ptr<T> CreateRect(const std::string& objectName,
		const SDL_Rect& rect,Vector2F* pos,SDL_Color color, MoveType mType)
	{
		auto object = std::make_shared<T>(objectName);
		objectManager->AddObject(object);
		object->MakeRect(renderer,rect,color);
		object->InitMove(pos[0], pos[1], mType);
		return object;
	}
	template <typename T>
	std::shared_ptr<T> CreateMoveTargetRect(const std::string& objectName,
		const SDL_Rect& rect, Vector2F* pos, SDL_Color color, MoveType mType)
	{
		auto object = CreateRect<T>(objectName, rect, pos, color, mType);
		Vector2F targetPos = pos[2];
		object->PushTarget(targetPos);
		return object;
	}

	bool CreateObjectsFromFile(const std::string& fileName);
	bool ReadMap(const std::string& mapName);
private:
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

