
#include "Game.h"

Game::Game() :
    objectManager{ new ObjectManager{}},
    eventHandler{ new EventHandler{} }
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
    window = SDL_CreateWindow("Square ManiaQ",
        100, 100,
        512, 768, SDL_WINDOW_OPENGL);

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
    // set program icon
    SDL_Surface* icon= IMG_Load("Data/icon.png");
    SDL_SetWindowIcon(window, icon);
    

    //init image
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
    {
        SDL_Log("failed to init image : %s", SDL_GetError());
        return false;
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
    SDL_Log("Splash Screen");
    static Uint32 splashTick{ 0 };
    static Uint32 splashcurTick{ 0 };
    //add splash logo object
    auto splash{ std::make_shared<GameObject>()};
    splash->LoadImage(renderer,"Data/splash.png");
    objectManager->AddObject(splash);
    splash->setPos(512 / 2, 768 / 2);
    //set alpha to 240
    render(60);
    while (splashTick < 150)
    {
        float deltaTime = SDL_GetTicks() - splashcurTick / 1000.0f;
        if (deltaTime > 0.05f)
            deltaTime = 0.05f;
        splashcurTick = SDL_GetTicks();
        if (splashTick > 60)
        {
            splash->setPos(512 / 2, 768  / 2 - ( 2 * splashTick * (splashTick - 60) * deltaTime));
            render();
        }
        else if (splashTick <= 60)
        {
            render(60 - splashTick);
        }
        ++splashTick;
        SDL_Delay(20);
    }
    objectManager->DeleteObject(splash);
    SDL_Log("Splash End");
    Event e;
    e.eid = EID::TITLE;
    eventHandler->PushEvent(std::make_shared<Event>(e));
}

void Game::run()
{
    while (bRun)
    {
        event();
        input();
        update();
        render();
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
            splash();
            gameState = GameState::SPLASH;
            break;
        case EID::TITLE:
            title();
            gameState = GameState::TITLE;
            break;
        }
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
            quit();
            break;
        }
    }
}

void Game::render(int fade)
{
    SDL_RenderClear(renderer);
    for (auto object : objectManager->objects())
    {
        SDL_Rect rect = object->imageRect();
        SDL_Rect posRect = object->posRect();
        SDL_QueryTexture(object->texture(), NULL, NULL, &rect.w,
            &rect.h);
        SDL_SetTextureBlendMode(object->texture(), SDL_BLENDMODE_BLEND);
        if (4 * fade > 255)
        {
            fade = 255 / 4;
        }
        SDL_SetTextureAlphaMod(object->texture(), 255 - 4 * fade);
        if (SDL_RenderCopy(renderer, object->texture(),
            &rect, &posRect) != 0)
        {
            SDL_Log("Render Failed: %s",SDL_GetError());
        }
    }
    SDL_RenderPresent(renderer);

}
void Game::title()
{
    SDL_Log("Title");
    auto titleLogo = std::make_shared<GameObject>();
    titleLogo->LoadImage(renderer, "Data/title.png");
    objectManager->AddObject(titleLogo);
    titleLogo->setPos(10 - 256, 10);

    auto startButton = std::make_shared<Button>();

}
void Game::update()
{
    float deltaTime = SDL_GetTicks() - curTick / 1000.0f;
    if (deltaTime > 0.05f)
    {
        deltaTime = 0.05f;
    }
    curTick = SDL_GetTicks();
}

