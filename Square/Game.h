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
	LOAD,
	RUN_ENTER,
	RUN,
	RUN_END,
	EDITOR,
	END
};

enum class EditorMode
{
	NEUTRAL,
	PLACE_WALL,
	PLACE_HOSTILE,
	PLACE_START_POS,
	PLACE_END_POS,
	SET_PATH,
	REVERT,
	REDO
};

class Game
{
public:
	Game();
	bool init();
	void run();
	void quit();
private:
	void event();
	//============================
	//Event related Function
	//============================

	//Queues event to game.
	void QueueEvent(EID e, const std::string& info = {});
	//Binds event to button.
	void BindEvent(std::shared_ptr<Button>const& b, EID e, const std::string& info = {});
	//Menu select Function.
	void MenuSelect();

	// makes to behave alongs to the state of game.
	void state();
	// reads input from the keyboard device.
	void input();
	// input event left click.
	void OnLeftClick();
	void OnMouseMove();
	void update();

	void render();
	void renderTextureFadeOut(std::shared_ptr<GameObject> object);
	void renderTextureFadeIn(std::shared_ptr<GameObject> object);

	//==============================
	// Game Behaviours
	//==============================

	// Intro screen
	void splash();
	void titleEnter();
	void title();
	void interactButton(std::shared_ptr<Button> const& button);
	void titleEnd(const std::string& info);
	void load(const std::string& info);
	void runEnter();
	//=============================
	// Editor Behaviours
	void editor();
	void ChangeMode(EditorMode mode);
	//===========================
	// Macro Function
	//===========================
	// check if string is number.
	bool isnumber(const std::string& str);
	// binds all button events when the editor loads.
	void editorBind();
	// enables all button.
	void enableButtons();
private:
	SDL_Window* window{};
	SDL_Renderer* renderer{};

	std::unique_ptr<ObjectManager> objectManager;
	std::unique_ptr<EventHandler> eventHandler;
	std::unique_ptr<ObjectFactory> objectFactory;

	bool bRun{ true };
	//====================
	// Variables for game loop
	//====================
	int tick{ 0 };
	int fadeTick{ 0 };
	bool loadInit{ false };
	bool runInit{ false };

	//====================
	// Variables for editor
	bool isPlacing{ false };
	Vector2 placeStartPos{};
	Vector2 placeEndPos{};
	int objectCount;
	//========================
	Uint32 curTick{};
	GameState gameState{ GameState::START };
	EditorMode editorMode{ EditorMode::NEUTRAL };
	Vector2 _mouse{};

	const int scrX{ 512 };
	const int scrY{ 768 };

	int cameraX{ 0 };
	int cameraY{ 0 };

	bool stageSelectCalled{ false };
	std::unordered_map<std::string, bool> buttonActive;
	std::vector<bool> stageCleared = {
		true,false,false,false,false,false,
		false,false,false,false,false,false,
		false,false,false,false,false,false,
		false,false,false,false,false,false,
		false,false,false,false,false,false,
	};
	std::string currentEventInfo{};

	const double rad_MAX = 2 * M_PI;
	const double rad_MIN = 0;
	int mapIndex = 0;
};
