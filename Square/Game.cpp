#include "Game.h"
#include "SDL/SDL_ttf.h"

#include "Component.h"
#include "SpriteComponent.h"

#include <cctype>
#include <algorithm>

Game::Game() :
	objectManager{ new ObjectManager{} },
	eventHandler{ new EventHandler{} },
	objectFactory{ new ObjectFactory{} }
{
}

bool Game::init()
{
	//init sdl
	int sdlInit = SDL_Init(SDL_INIT_EVERYTHING);
	if (sdlInit != 0)
	{
		SDL_Log("init failed : %s", SDL_GetError());
		return false;
	}
	//create window
	window = SDL_CreateWindow("Square Maniac",
		100, 50,
		scrX, scrY, SDL_WINDOW_OPENGL);

	if (!window)
	{
		SDL_Log("failed to create window : %s", SDL_GetError());
		return false;
	}
	//create renderer
	renderer = SDL_CreateRenderer(window,
		-1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!renderer)
	{
		SDL_Log("failed to create renderer: %s", SDL_GetError());
		return false;
	}

	//init image
	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
	{
		SDL_Log("failed to init image : %s", SDL_GetError());
		return false;
	}
	// set program icon
	SDL_Surface* icon = IMG_Load("Data/icon.png");
	SDL_SetWindowIcon(window, icon);

	//init TTF
	if (TTF_Init() < 0)
	{
		SDL_Log("TTF init failed! : %s", SDL_GetError());
	}
	// Set Background to white
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);
	// push splash event
	Event e;
	e.eid = EID::SPLASH;
	eventHandler->PushEvent(std::make_shared<Event>(e));
	return true;
}

void Game::splash()
{
	auto splash = objectManager->find("splash");
	auto square = objectManager->find("square");
	if (tick == 0)
	{
		splash->InitFade(Fade::OUT, 60, 4);
	}
	if (tick > 60)
	{
		splash->SetVelocity({ 0,(tick - 60) * -1 / 4 });
		splash->SetRotateAmount(5);
		square->SetRotateAmount(20);
		SDL_FRect squareHitbox = square->hitbox();
		if (squareHitbox.y <= scrY / 2.f - 30)
			square->SetVelocity({ 0,(tick - 60) / 4 });
		else
			square->SetVelocity({ 0,0 });
	}
	if (tick > 150)
	{
		SDL_Log("Splash End");
		Event e;
		e.eid = EID::TITLE_ENTER;
		eventHandler->PushEvent(std::make_shared<Event>(e));
		objectManager->DeleteObject(splash);
	}
	++tick;
}

void Game::run()
{
	while (bRun)
	{
		event();
		state();
		input();
		render();
		update();
		SDL_Delay(10);
	}
}

void Game::quit()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}

//deals with non-SDL event
void Game::event()
{
	while (auto e = eventHandler->PopEvent())
	{
		if (!e->info.empty())
			currentEventInfo = e->info;
		switch (e->eid)
		{
		case EID::SPLASH:
			tick = 0;
			objectFactory->CreateObjectsFromFile(objectManager, renderer, "Objects/splash.obl");
			gameState = GameState::SPLASH;
			break;
		case EID::TITLE_ENTER:
			tick = 0;
			gameState = GameState::TITLE_ENTER;
			break;
		case EID::TITLE:
			gameState = GameState::TITLE;
			break;
		case EID::TITLE_STAGE_SELECT_CALLED:
		{
			if (!stageSelectCalled)
			{
				MenuSelect();
			}
			break;
		}
		case EID::TITLE_END:
			tick = 0;
			gameState = GameState::TITLE_END;
			break;
		case EID::LOAD:
			gameState = GameState::LOAD;
			break;
		case EID::LOAD_COMPLETE:
			gameState = GameState::RUN_ENTER;
			break;
		//Editor zone
		case EID::EDITOR:
			gameState = GameState::EDITOR;
			break;
		case EID::EDITOR_BUTTON_WALL:
			SDL_Log("Wall button!");
			ChangeMode(EditorMode::PLACE_WALL);
			break;
		
		case EID::GAME:
			gameState = GameState::RUN;
			break;
		case EID::GO_TO_TITLE:
			objectManager->Flush();
			tick = 0;
			auto capsule = objectFactory->CreateObjectCapsule(
				"square", "Data/null.png", { scrX / 2.f - 30, scrY / 2.f - 30 }, { 0,0 }, {}, MoveType::SQUARE);
			auto rect = objectFactory->CreateRectObjectCapsule(capsule, { 60,60 }, { 0,0,0,255 });
			objectFactory->CreateRect<GameObject>(objectManager, renderer, rect);
			auto square = objectManager->find("square");
			square->SetRotateAmount(20);
			gameState = GameState::TITLE_ENTER;
			break;
		}
	}
}

void Game::QueueEvent(EID e, const std::string& info)
{
	Event event;
	event.eid = e;
	event.info = info;
	eventHandler->PushEvent(std::make_shared<Event>(e));
}

//Binds
void Game::BindEvent(std::shared_ptr<GameObject>const& b, EID e, const std::string& info)
{
	Event event;
	event.eid = e;
	event.info = info;
	b->BindEvent(event);
}

void Game::MenuSelect()
{
	auto buffer = objectFactory->CreateObjectCapsule("stageSelect"
		, "Data/stagebackground.png", { scrX / 2.f - 150,scrY / 2.f - 150 }, { 0,0 }, {}, MoveType::DEFAULT);
	auto stageSelect = objectFactory->CreateObject<GameObject>(objectManager, renderer, buffer);

	for (int i = 0; i < stageCleared.size(); ++i)
	{
		buffer = objectFactory->CreateObjectCapsule("stageButton" + std::to_string(1 + i),
			"Data/Fonts/font.ttf", { scrX / 2.f - 150 + 50 * (i % 6), scrY / 2.f - 150 + 5 + 50 * (i / 6) },
			{ 0,0 }, {}, MoveType::DEFAULT);
		auto textBuffer = objectFactory->CreateTextObjectCapsule(buffer, Text{ 36,{255,255,255},std::to_string(1 + i) });
		auto stageButton = objectFactory->CreateTextObject<TextButton>(objectManager, renderer, textBuffer);
		BindEvent(stageButton, EID::TITLE_END, std::to_string(i + 1));
	}
	stageSelect->InitFade(Fade::FADE_OUT, 60, 6);
	stageSelectCalled = true;
}

void Game::state()
{
	switch (gameState)
	{
	case GameState::SPLASH:
		splash();
		break;
	case GameState::TITLE_ENTER:
		loadInit = false;
		runInit = false;
		titleEnter();
		break;
	case GameState::TITLE:
		title();
		break;
	case GameState::TITLE_END:
		titleEnd(currentEventInfo);
		break;
	case GameState::LOAD:
	{
		if (!loadInit)
		{
			load(currentEventInfo);
			loadInit = true;
		}
		break;
	}
	case GameState::RUN_ENTER:
		runEnter();
		break;
	case GameState::EDITOR:
		editor();
		break;
	}
}

//deals with SDL-related Event (inputs)
void Game::input()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			bRun = false;
			quit();
			break;
		case SDL_MOUSEBUTTONDOWN:
			switch (event.button.button)
			{
			case SDL_BUTTON_LEFT:
				OnLeftClick();
				break;
			}

			break;
		case SDL_MOUSEMOTION:
		{
			OnMouseMove();
			break;
		}
		}
	}

	SDL_GetMouseState(&_mouse.x, &_mouse.y);
}

void Game::OnLeftClick()
{
	for (auto object : objectManager->objects())
	{
		if (!object)
			SDL_Log("empty!");
		if (auto button = std::dynamic_pointer_cast<Button>(object))
		{
			if (button->is_hover(_mouse) && button->isEnabled() && !button->hasTarget())
			{
				button->ClickEvent(eventHandler);
				buttonActive.erase(button->name());
				button->disable();
			}
		}

		if (auto object = objectManager->find("stageSelect"))
		{
			if (!object->is_hover(_mouse) && stageSelectCalled)
			{
				object->InitFade(Fade::FADE_IN, 60, 6);
				objectManager->DeleteObject(object);
				for (int i = 0; i < stageCleared.size(); ++i)
				{
					auto object = objectManager->find("stageButton" + std::to_string(i + 1));
					objectManager->DeleteObject(object);
				}
				stageSelectCalled = false;
				// enables start button and makes start button to go original position.
				auto startButton = std::dynamic_pointer_cast<Button>(objectManager->find("titleStartButton"));
				startButton->enable();
				buttonActive[startButton->name()] = true;
			}
		}
	}
	if (gameState == GameState::EDITOR)
	{
		switch (editorMode)
		{
		case EditorMode::NEUTRAL:
			break;
		case EditorMode::PLACE_WALL:
			SDL_GetMouseState(&placeStartPos.x, &placeStartPos.y);

			if (!isPlacing)
			{
				SDL_Log("placing mode.");
				++objectCount;
				isPlacing = true;
			}
			else
			{
				SDL_Log("Neutral");
				isPlacing = false;
				editorMode = EditorMode::NEUTRAL;
			}
			break;
		}
	}
}

void Game::OnMouseMove()
{
	SDL_GetMouseState(&placeEndPos.x, &placeEndPos.y);
	Vector2 tmp = placeEndPos;
	if (placeStartPos.x > placeEndPos.x)
	{
		placeEndPos = placeStartPos;
		placeStartPos.x = tmp.x;
	}
	if (placeStartPos.y > placeEndPos.y)
	{
		placeEndPos = placeStartPos;
		placeStartPos.y = tmp.y;
	}
	switch (editorMode)
	{
	case EditorMode::NEUTRAL:
		break;
	case EditorMode::PLACE_WALL:
		if (isPlacing)
		{
			auto capsule = objectFactory->CreateObjectCapsule(
				"Wall" + std::to_string(objectCount), "Data/null.png", {static_cast<float>(placeStartPos.x)
				,static_cast<float>(placeStartPos.y) }, { 0,0 }, {}, MoveType::DEFAULT
			);
			auto rect = objectFactory->CreateRectObjectCapsule(capsule,
				{ std::abs(placeEndPos.x - placeStartPos.x),std::abs(placeEndPos.y - placeStartPos.y) }, { 0,0,0,255 });
			objectFactory->CreateRect<GameObject>(objectManager, renderer, rect);
			auto wall = objectManager->find("Wall" + std::to_string(objectCount));
		}
	break;
	}
}

void Game::render()
{
	SDL_RenderClear(renderer);
	for (auto object : objectManager->objects())
	{
		auto spriteComponent = object->GetComponent<SpriteComponent>();
		if (auto sprite = spriteComponent.lock())
			sprite->draw(renderer);
	}
	SDL_RenderPresent(renderer);
}
void Game::titleEnter()
{
	if (tick == 0)
	{
		SDL_Log("Title");
		objectFactory->CreateObjectsFromFile(objectManager, renderer, "Objects/title.obl");
	}
	auto titleLogo = objectManager->find("titleLogo");
	auto startButton = std::dynamic_pointer_cast<Button>(objectManager->find("titleStartButton"));
	auto editorButton = std::dynamic_pointer_cast<Button>(objectManager->find("editorButton"));
	if (tick < 60)
	{
		titleLogo->SetVelocity({ 0,tick / 3 * -1 });
		startButton->SetVelocity({ 0,tick / 3 * -1 });
		editorButton->SetVelocity({ 0,tick / 3 * -1 });
	}

	if (titleLogo->isTargetEmpty() && startButton->isTargetEmpty())
	{
		QueueEvent(EID::TITLE);
		SDL_Log("title entry done.");
		BindEvent(startButton, EID::TITLE_STAGE_SELECT_CALLED);
		BindEvent(editorButton, EID::TITLE_END, "editor");
	}
	auto square = objectManager->find("square");
	++tick;
}
void Game::title()
{
	for (auto object : objectManager->objects())
	{
		if (std::shared_ptr<Button> startButton = std::dynamic_pointer_cast<Button>(
			objectManager->find("titleStartButton")))
		{
			interactButton(startButton);
		}
		if (std::shared_ptr<Button> editorButton = std::dynamic_pointer_cast<Button>
			(objectManager->find("editorButton")))
		{
			interactButton(editorButton);
		}
		// set StageButtons
		if (objectManager->name_contains(object, "stageButton"))
		{
			auto stageButton = std::dynamic_pointer_cast<TextButton>(object);
			if (object->is_hover(_mouse))
				stageButton->SetColor(renderer, { 255,0,0 });
			else
				stageButton->SetColor(renderer, { 255,255,255 });
		}
	}
}
void Game::interactButton(std::shared_ptr<GameObject> const& button)
{
	auto buttonOverlay = objectManager->find(button->name() + "Overlay");
	std::string fileNameNoExtender = button->fileName().substr(0, button->fileName().size() - 4);
	if (button->isEnabled())
	{
		if (button->is_hover(_mouse))
		{
			if (!buttonActive[button->name()])
			{
				if (button->isTargetEmpty() && buttonOverlay->isTargetEmpty()) {
					button->LoadImage(renderer, fileNameNoExtender + "Active.png");
					buttonOverlay->LoadImage(renderer, fileNameNoExtender + "Overlay.png");
					SDL_FRect hitbox = button->hitbox();
					hitbox.w = hitbox.w + 64;
					SDL_FRect ogPos = button->posRect();
					button->PushTarget({ ogPos.x - 30,ogPos.y });
					buttonOverlay->PushTarget({ ogPos.x - 30,ogPos.y });
					button->SetVelocity({ -10,0 });
					buttonOverlay->SetVelocity({ -10,0 });
					button->SetHitbox(hitbox);
					buttonActive[button->name()] = true;
				}
			}
		}
		else
		{
			if (button->isTargetEmpty() && buttonOverlay->isTargetEmpty())
			{
				if (buttonActive[button->name()])
				{
					button->LoadImage(renderer, fileNameNoExtender.substr(0, fileNameNoExtender.size() - 6) + ".png");
					buttonOverlay->LoadImage(renderer, "Data/null.png");
					SDL_FRect hitbox = button->hitbox();
					hitbox.w = 128;
					SDL_FRect ogPos = button->posRect();
					button->PushTarget({ ogPos.x + 30,ogPos.y });
					button->SetVelocity({ 10,0 });
					buttonOverlay->PushTarget({ ogPos.x + 30,ogPos.y });
					buttonOverlay->SetVelocity({ 10,0 });
					button->SetHitbox(hitbox);
					buttonActive[button->name()] = false;
				}
			}
		}
	}
}
void Game::titleEnd(const std::string& info)
{
	for (auto object : objectManager->objects())
	{
		if (object == objectManager->find("square"))
		{
			object->SetVelocity({ 0, tick });
			object->SetRotateAmount(20 + tick);
		}
		else
		{
			object->SetVelocity({ 0, tick * -1 });
		}
		object->SetMoveType(MoveType::DEFAULT);
		if (object->hitbox().y < 0 - object->imageRect().h
			|| object->hitbox().y > scrY + object->imageRect().h)
		{
			objectManager->DeleteObject(object);
		}
	}
	tick += 4;
	if (objectManager->objects().empty())
	{
		SDL_SetRenderDrawColor(renderer, 255 - fadeTick,
			255 - fadeTick, 255 - fadeTick, 255);
		if (fadeTick > 255)
			fadeTick = 255;
		else if (fadeTick == 255)
		{
			QueueEvent(EID::LOAD, info);
			fadeTick = 0;
		}
		else
			fadeTick += 8;
	}
}
void Game::load(const std::string& info)
{
	//check the info contains stage number.
	if (isnumber(info))
	{
		SDL_Log("hi!");
	}
	else if (info == "editor")
	{
		SDL_Log("editor");
		objectFactory->CreateObjectsFromFile(objectManager, renderer, "Objects/editor.obl");
		editorBind();
	}
	//hides all objects and makes to stop moving.
	objectManager->SetVisibleAll(false);
	objectManager->DeactivateAll();
	QueueEvent(EID::LOAD_COMPLETE);
}
void Game::runEnter()
{
	if (!runInit)
	{
		objectManager->SetVisibleAll(true);
		objectManager->ActivateAll();
		runInit = true;
	}
	else
	{
		// Set screen slowly white.
		SDL_SetRenderDrawColor(renderer, fadeTick, fadeTick, fadeTick, 255);
		if (fadeTick > 255)
			fadeTick = 255;
		else if (fadeTick == 255)
		{
			if (currentEventInfo == "editor")
				QueueEvent(EID::EDITOR);
			else
				QueueEvent(EID::GAME);
			fadeTick = 0;
		}
		else
			fadeTick += 8;
	}
}
void Game::editor()
{
	for (auto object : objectManager->objects())
	{
		if (object == objectManager->find("editorQuitButton"))
		{
			interactButton(std::dynamic_pointer_cast<Button>(object));
		}
	}
}
void Game::ChangeMode(EditorMode mode)
{
	//enables all buttons
	enableButtons();
	if (editorMode == mode)
		return;
	editorMode = mode;
}
void Game::update()
{
	float deltaTime = SDL_GetTicks() - curTick / 1000.0f;
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}
	curTick = SDL_GetTicks();
	for (const auto& object : objectManager->objects())
	{
		if (object->isActive())
		{
			object->Move(deltaTime);
			object->MoveTargetted(deltaTime);
			object->Rotate();
		}
	}
}
bool Game::isnumber(const std::string& str)
{
	return std::all_of(str.begin(), str.end(), ::isdigit);
}

void Game::editorBind()
{
	auto editorQuitButton = std::dynamic_pointer_cast<Button>(objectManager->find("editorQuitButton"));
	BindEvent(editorQuitButton, EID::GO_TO_TITLE);
	auto editorWallButton = std::dynamic_pointer_cast<Button>(objectManager->find("editorWallButton"));
	BindEvent(editorWallButton, EID::EDITOR_BUTTON_WALL);
	auto editorHostileButton = std::dynamic_pointer_cast<Button>(objectManager->find("editorHostileButton"));
	BindEvent(editorHostileButton, EID::EDITOR_BUTTON_HOSTILE);
	auto editorStartPosButton = std::dynamic_pointer_cast<Button>(objectManager->find("editorStartPosButton"));
	BindEvent(editorStartPosButton, EID::EDITOR_BUTTON_SP);
	auto editorEndPosButton = std::dynamic_pointer_cast<Button>(objectManager->find("editorEndPosButton"));
	BindEvent(editorEndPosButton, EID::EDITOR_BUTTON_EP);
	auto editorPathButton = std::dynamic_pointer_cast<Button>(objectManager->find("editorPathButton"));
	BindEvent(editorPathButton, EID::EDITOR_BUTTON_PATH);
	auto editorBackButton = std::dynamic_pointer_cast<Button>(objectManager->find("editorBackButton"));
	BindEvent(editorBackButton, EID::EDITOR_BUTTON_BACK);
	auto editorForwardButton = std::dynamic_pointer_cast<Button>(objectManager->find("editorForwardButton"));
	BindEvent(editorForwardButton, EID::EDITOR_BUTTON_FORWARD);
}

void Game::enableButtons()
{
	for (auto object : objectManager->objects())
	{
		if (auto button = std::dynamic_pointer_cast<Button>(object))
			button->enable();
	}
}
