
#include "Game.h"
#include "Button.h"
#include "TextButton.h"
#include "SDL/SDL_ttf.h"

#include <fstream>

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

    // push splash event
    Event e;
    e.eid = EID::SPLASH;
    eventHandler->PushEvent(std::make_shared<Event>(e));
    return true;
}

void Game::splash()
{
    static int splashTick{ 0 };
    static Uint32 splashcurTick{ 0 };
    auto splash = objectManager->find("splash");
    auto square = objectManager->find("square");
    //add splash logo object
    if (splashTick == 0)
    {
        auto buffer = objectFactory->CreateObjectCreateBuffer("splash", "Data/splash.png",
            { scrX / 2.f,scrY / 2.f }, { 0,0 }, {}, MoveType::SQUARE);
        auto splash = objectFactory->CreateObject<GameObject>(objectManager,renderer,buffer);
        auto square = objectFactory->CreateMoveTargetRect<GameObject>(objectManager,renderer,"square",
            SDL_Rect{ 0,0,60,60 },
            new Vector2F[3]{ {scrX / 2.f - 30,0} ,{0,0},
            {scrX / 2.f - 30, scrY / 2.f} },
            SDL_Color{ 0,0,0 },
            MoveType::SQUARE);
        //set alpha to 240
        splash->InitFade(Fade::FADE_OUT, 60, 4);
    }
    if (splashTick > 60)
    {
        splash->SetVelocity({ 0,(splashTick - 60) * -1 / 4 });
        splash->SetRotateAmount(5);
        square->SetRotateAmount(20);
        SDL_FRect squareHitbox = square->hitbox();
        if (squareHitbox.y <= scrY / 2.f - 30)
        {
            square->SetVelocity({ 0,(splashTick - 60) / 4 });
        }
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
                auto buffer = objectFactory->CreateObjectCreateBuffer("stageSelect"
                    , "Data/stagebackground.png", { scrX / 2.f - 150,scrY / 2.f - 150 },{ 0,0 }, {}, MoveType::DEFAULT);
                auto stageSelect = objectFactory->CreateObject<GameObject>(objectManager,renderer,buffer);

                for (int i = 0; i < stageCleared.size(); ++i)
                {
                    auto stageButton = objectFactory->CreateTextObject<TextButton>
                        (
                            objectManager,renderer,
                            "stageButton" + std::to_string(1 + i),
                            "Data/Fonts/font.ttf",
                            { 36,{255,255,255},std::to_string(1 + i) },
                            new Vector2F[2]{ {scrX / 2.f - 150 + 50 * (i % 6) ,scrY / 2.f - 150 + 5 + 50 * (i / 6)},{0,0}},
                            MoveType::DEFAULT
                        );
                    Event* e = new Event;
                    e->eid = EID::TITLE_END;
                    stageButton->BindEvent(e);
                }
                stageSelect->InitFade(Fade::FADE_OUT, 60, 6);
                stageSelectCalled = true;
            }
            break;
        }
        case EID::TITLE_END:
            gameState = GameState::TITLE_END;
            break;
        }
    }
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
        titleEnd();
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
            {
                for (auto object : objectManager->objects())
                {
                    if (!object)
                        SDL_Log("empty!");
                    if (auto button = std::dynamic_pointer_cast<Button>(object))
                    {
                        if (button->is_hover(_mouse))
                        {
                            button->ClickEvent(eventHandler);
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
                        }
                    }
                    if (objectManager->name_contains(object,"stageButton"))
                    {
                        if (object->is_hover(_mouse))
                        {
                            mapIndex = std::stoi(object->objectName().substr(11));
                            SDL_Log("%d", mapIndex);
                        }
                    }
                }
            }
            }
            break;
        }
    }

    SDL_GetMouseState(&_mouse.x, &_mouse.y);

}

void Game::render()
{
    SDL_RenderClear(renderer);
    for (auto object : objectManager->objects())
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
    SDL_RenderPresent(renderer);

}

void Game::renderTextureFadeOut(std::shared_ptr<GameObject> object)
{
    int currentFade = object->currentFade();
    const int fadeAmount = object->fadeAmount();
    if (fadeAmount * currentFade > 255)
    {
        currentFade = 255 / fadeAmount;
    }
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
    {
        currentFade = 255 / fadeAmount;
    }
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
    static bool isButtonActive{ false };
    if (!init)
    {
        SDL_Log("Title");
        auto buffer = objectFactory->CreateObjectCreateBuffer("titleLogo",
            "Data/title.png", { 10,896 }, { 0,-10 }, { 10,0 }, MoveType::SQUARE);
        objectFactory->CreateMoveTargetObject<GameObject>(objectManager,renderer,buffer);
        buffer = objectFactory->CreateObjectCreateBuffer
        ("titleStartButtonOverlay", "Data/null.png",{ 374,250 }, { 0,0 }, {}, MoveType::SQUARE);
        objectFactory->CreateObject<GameObject>(objectManager,renderer,buffer);
        buffer = objectFactory->CreateObjectCreateBuffer("titleStartButton", "Data/startbutton.png",
            { 374,796 }, { 0,-1 }, { 374,250 },MoveType::SQUARE);
        objectFactory->CreateMoveTargetObject<Button>(
            objectManager,renderer,buffer);
        buffer = objectFactory->CreateObjectCreateBuffer("optionButton", "Data/option.png",
            {10,scrY - 64 - 10.f},{0,0} , {}, MoveType::SQUARE);
        objectFactory->CreateObject<Button>(objectManager,renderer,buffer);
        init = true;
    }
    auto titleLogo = objectManager->find("titleLogo");
    auto startButton = std::dynamic_pointer_cast<Button>(objectManager->find("titleStartButton"));
    if (titleTick < 60)
    {
        titleLogo->SetVelocity({ 0,titleTick /3 * -1 });
        startButton->SetVelocity({ 0,titleTick / 3 * - 1 });
    }

    if (titleLogo->isTargetEmpty() && startButton->isTargetEmpty())
    {
        Event e;
        e.eid = EID::TITLE;
        eventHandler->PushEvent(std::make_shared<Event>(e));
        SDL_Log("title entry done.");
        Event* ce = { new Event };
        ce->eid = EID::TITLE_STAGE_SELECT_CALLED;
        startButton->BindEvent(ce);
    }
    auto square = objectManager->find("square");
    ++titleTick;
}
void Game::title()
{
    bool isStartButtonActive{ false };
    for (auto object : objectManager->objects())
    {
        if (std::shared_ptr<Button> startButton = std::dynamic_pointer_cast<Button>(
        objectManager->find("titleStartButton")))
        {
            auto startButtonOverlay = objectManager->find("titleStartButtonOverlay");
            if (startButton->is_hover(_mouse))
            {
                {
                    if(startButton->isTargetEmpty() && startButtonOverlay->isTargetEmpty()){
                        startButton->LoadImage(renderer,"Data/startbuttonActive.png");
                        startButtonOverlay->LoadImage(renderer, "Data/startbuttonActiveOverlay.png");
                        SDL_FRect hitbox = startButton->hitbox();
                        hitbox.w = 200;
                        startButton->PushTarget({ 344,250 });
                        startButtonOverlay->PushTarget({ 344,250 });
                        startButton->SetVelocity({ -10,0 });
                        startButtonOverlay->SetVelocity({ -10,0});
                        startButton->SetHitbox(hitbox);
                        isStartButtonActive = true;
                    }
                }
            }
            else
            {
                if (startButton->isTargetEmpty() && startButtonOverlay->isTargetEmpty())
                {
                    startButton->LoadImage(renderer, "Data/startbutton.png");
                    startButtonOverlay->LoadImage(renderer, "Data/null.png");
                    SDL_FRect hitbox = startButton->hitbox();
                    hitbox.w = 128;
                    startButton->PushTarget({ 374,250 });
                    startButton->SetVelocity({ 10,0 });
                    startButtonOverlay->PushTarget({ 374,250 });
                    startButtonOverlay->SetVelocity({ 10,0 });
                    startButton->SetHitbox(hitbox);
                    isStartButtonActive = false;
                }
            }
        }
        if (objectManager->name_contains(object,"stageButton"))
        {
            auto stageButton = std::dynamic_pointer_cast<TextButton>(object);
            if (object->is_hover(_mouse))
            {
                stageButton->SetColor(renderer,{ 255,0,0 });
            }
            else
            {
                stageButton->SetColor(renderer,{ 255,255,255 });
            }
        }
    }
}
void Game::titleEnd()
{
    static int titleEndTick{ 0 } ;
    for (auto object : objectManager->objects())
    {
        if (object == objectManager->find("square"))
        {
            object->SetVelocity({ 0, titleEndTick++ / 16 });
            object->SetRotateAmount(40);
        }
        else
        {
            object->SetVelocity({ 0, titleEndTick++ / 16 * -1 });
        }
        object->SetMoveType(MoveType::DEFAULT);
        if (object->hitbox().y < 0 - object->imageRect().h
            || object->hitbox().y > scrY + object->imageRect().h)
        {
                objectManager->DeleteObject(object);
                //break;
        }

    }
    if (objectManager->objects().empty())
    {
        static int fadeTick{ 0 };
        if (fadeTick > 255)
        {
            fadeTick = 255;
            return;
        }
        SDL_SetRenderDrawColor(renderer, 255 - fadeTick,
            255 - fadeTick , 255 - fadeTick, 255);
        fadeTick+= 8;
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
        object->Move(deltaTime);
        object->MoveTargetted(deltaTime);
        object->Rotate();
    }
}

