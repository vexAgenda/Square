
#include "Game.h"
#include "Button.h"
#include "TextButton.h"
#include "SDL/SDL_ttf.h"

#include <cctype>
#include <algorithm>

Game::Game() :
    objectManager{ new ObjectManager{}},
    eventHandler{ new EventHandler{} },
    objectFactory{ new ObjectFactory{}}
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

    objectFactory->CreateObjectsFromFile(objectManager, renderer, "Objects/splash.obl");
    // push splash event
    Event e;
    e.eid = EID::SPLASH;
    eventHandler->PushEvent(std::make_shared<Event>(e));
    return true;
}

void Game::splash()
{
    static int splashTick{ 0 };
    auto splash = objectManager->find("splash");
    auto square = objectManager->find("square");
    if (splashTick == 0)
    {
        splash->InitFade(Fade::FADE_OUT, 60, 4);
    }
    if (splashTick > 60)
    {
        splash->SetVelocity({ 0,(splashTick - 60) * -1 / 4 });
        splash->SetRotateAmount(5);
        square->SetRotateAmount(20);
        SDL_FRect squareHitbox = square->hitbox();
        if (squareHitbox.y <= scrY / 2.f - 30)
            square->SetVelocity({ 0,(splashTick - 60) / 4 });
        else
            square->SetVelocity({ 0,0 });
    }
    if (splashTick > 150)
    {
        SDL_Log("Splash End");
        Event e;
        e.eid = EID::TITLE_ENTER;
        eventHandler->PushEvent(std::make_shared<Event>(e));
        objectManager->DeleteObject(splash);
    }
    ++splashTick;
}

void Game::run()
{
    while (bRun)
    {
        event();
        state();
        input();
        update();
        render();
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
            gameState = GameState::SPLASH;
            break;
        case EID::TITLE_ENTER:
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
            gameState = GameState::TITLE_END;
            break;
        case EID::LOAD:
            gameState = GameState::LOAD;
            break;
        case EID::LOAD_COMPLETE:
            gameState = GameState::RUN_ENTER;
            break;
        }
    }
}

void Game::QueueEvent(EID e,const std::string& info)
{
    Event event;
    event.eid = e;
    event.info = info;
    eventHandler->PushEvent(std::make_shared<Event>(e));
}

//Binds
void Game::BindEvent(std::shared_ptr<Button>const& b, EID e, const std::string& info)
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
        BindEvent(stageButton, EID::TITLE_END,std::to_string(i + 1));
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
        static bool loadInit{ false };
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
            if (button->is_hover(_mouse))
                button->ClickEvent(eventHandler);
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
            }
        }
    }
}

void Game::render()
{
    SDL_RenderClear(renderer);
    for (auto object : objectManager->objects())
    {
        if (object->visible())
        {
            SDL_Rect rect = object->imageRect();
            SDL_FRect posRect = object->posRect();
            SDL_QueryTexture(object->texture(), NULL, NULL, &rect.w,
                &rect.h);
            SDL_SetTextureBlendMode(object->texture(), SDL_BLENDMODE_BLEND);
            switch (object->fadeType())
            {
            case Fade::FADE_IN:
                renderTextureFadeIn(object);
                break;
            case Fade::FADE_OUT:
                renderTextureFadeOut(object);
                break;
            }

            if (SDL_RenderCopyExF(renderer, object->texture(),
                &rect, &posRect, object->angle(), NULL, SDL_FLIP_NONE)
                != 0)
            {
                SDL_Log("Render Failed: %s", SDL_GetError());
            }
        }
    }
    SDL_RenderPresent(renderer);

}

void Game::renderTextureFadeOut(std::shared_ptr<GameObject> object)
{
    int currentFade = object->currentFade();
    const int fadeAmount = object->fadeAmount();
    if (fadeAmount * currentFade > 255)
        currentFade = 255 / fadeAmount;
    if (object->currentFade() > 0)
    {
        SDL_SetTextureAlphaMod(object->texture(), 255 - fadeAmount * currentFade);
        object->SetCurrentFade(currentFade - fadeAmount);
    }
}
void Game::renderTextureFadeIn(std::shared_ptr<GameObject> object)
{
    int currentFade = object->currentFade();
    const int fadeAmount = object->fadeAmount();
    if (fadeAmount * currentFade > 255)
        currentFade = 255 / fadeAmount;
    if (object->currentFade() > 0)
    {
        SDL_SetTextureAlphaMod(object->texture(), fadeAmount * currentFade);
        object->SetCurrentFade(currentFade - fadeAmount);
    } 
}
void Game::titleEnter()
{
    static bool init{ false };
    static int titleTick{ 0 };
    if (!init)
    {
        SDL_Log("Title");
        objectFactory->CreateObjectsFromFile(objectManager, renderer, "Objects/title.obl");
        init = true;
    }
    auto titleLogo = objectManager->find("titleLogo");
    auto startButton = std::dynamic_pointer_cast<Button>(objectManager->find("titleStartButton"));
    auto editorButton = std::dynamic_pointer_cast<Button>(objectManager->find("editorButton"));
    if (titleTick < 60)
    {
        titleLogo->SetVelocity({ 0,titleTick /3 * -1 });
        startButton->SetVelocity({ 0,titleTick / 3 * - 1 });
        editorButton->SetVelocity({ 0,titleTick / 3 * -1 });
    }

    if (titleLogo->isTargetEmpty() && startButton->isTargetEmpty())
    {
        QueueEvent(EID::TITLE);
        SDL_Log("title entry done.");
        BindEvent(startButton, EID::TITLE_STAGE_SELECT_CALLED);
        BindEvent(editorButton, EID::TITLE_END, "editor");
    }
    auto square = objectManager->find("square");
    ++titleTick;
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
        if (objectManager->name_contains(object,"stageButton"))
        {
            auto stageButton = std::dynamic_pointer_cast<TextButton>(object);
            if (object->is_hover(_mouse))
                stageButton->SetColor(renderer,{ 255,0,0 });
            else
                stageButton->SetColor(renderer,{ 255,255,255 });
        }
    }
}
void Game::interactButton(std::shared_ptr<Button> const& button)
{
    auto buttonOverlay = objectManager->find(button->objectName() + "Overlay");
    std::string fileNameNoExtender = button->fileName().substr(0,button->fileName().size() - 4);
    if (button->is_hover(_mouse))
    {
        if (!buttonActive[button->objectName()])
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
                buttonActive[button->objectName()] = true;
            }
        }
    }
    else
    {
        if (button->isTargetEmpty() && buttonOverlay->isTargetEmpty())
        {
            if (buttonActive[button->objectName()])
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
                buttonActive[button->objectName()] = false;
            }
        }
    }
}
void Game::titleEnd(const std::string& info)
{
    static int titleEndTick{ 0 } ;
    for (auto object : objectManager->objects())
    {
        if (object == objectManager->find("square"))
        {
            object->SetVelocity({ 0, titleEndTick});
            object->SetRotateAmount(40);
        }
        else
        {
            object->SetVelocity({ 0, titleEndTick * -1 });
        }
        object->SetMoveType(MoveType::DEFAULT);
        if (object->hitbox().y < 0 - object->imageRect().h
            || object->hitbox().y > scrY + object->imageRect().h)
        {
                objectManager->DeleteObject(object);
        }
    }
    titleEndTick++;
    if (objectManager->objects().empty())
    {
        static int fadeTick{ 0 };
        SDL_SetRenderDrawColor(renderer, 255 - fadeTick,
            255 - fadeTick , 255 - fadeTick, 255);
        if (fadeTick > 255)
            fadeTick = 255;
        else if (fadeTick == 255)
            QueueEvent(EID::LOAD, info);
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
    }
    objectManager->SetVisibleAll(false);
    objectManager->DeactivateAll();
    QueueEvent(EID::LOAD_COMPLETE);
}
void Game::runEnter()
{
    static bool runInit{ false };
    static int fadeTick{ 0 };
    if (!runInit)
    {
        objectManager->SetVisibleAll(true);
        objectManager->ActivateAll();
        runInit = true;
    }
    else
    {
        // Set screen slowly white.
        SDL_SetRenderDrawColor(renderer,fadeTick,fadeTick,fadeTick, 255);
        if (fadeTick > 255)
            fadeTick = 255;
        else if (fadeTick == 255)
            QueueEvent(EID::GAME);
        else
            fadeTick += 8;
    }
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

