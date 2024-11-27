
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
    static int splashTick{ 0 };
    static Uint32 splashcurTick{ 0 };
    //add splash logo object
    std::shared_ptr<GameObject> splash{ nullptr };
    if (splashTick == 0)
    {
        splash = CreateObject("splash", "Data/splash.png", 
            new Pos[2]{ { 512 / 2,768 / 2 }, { 0,splashTick } }, MoveType::EXPONENT);
        //set alpha to 240
        splash->SetTextureFade(60);
    }
    splash = objectManager->find("splash");
    if (splashTick > 60)
    {
        splash->SetMoveVelocity({ 0,(splashTick - 60) * -1 });
    }
    if (splashTick > 150)
    {
        SDL_Log("Splash End");
        Event e;
        e.eid = EID::TITLE;
        eventHandler->PushEvent(std::make_shared<Event>(e));
        objectManager->DeleteObject(splash);
    }
    SDL_Delay(20);
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
        case EID::TITLE:
            gameState = GameState::TITLE;
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
    case GameState::TITLE:
        title();
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
            quit();
            break;
        }
    }
}

void Game::render()
{
    SDL_RenderClear(renderer);
    for (auto object : objectManager->objects())
    {
        SDL_Rect rect = object->imageRect();
        SDL_Rect posRect = object->posRect();
        SDL_QueryTexture(object->texture(), NULL, NULL, &rect.w,
            &rect.h);
        SDL_SetTextureBlendMode(object->texture(), SDL_BLENDMODE_BLEND);
        int fade = object->textureFade();
        if (4 * fade > 255)
        {
            fade = 255 / 4;
        }
        if (object->textureFade() > 0)
        {
            SDL_SetTextureAlphaMod(object->texture(), 255 - 4 * fade);
            object->SetTextureFade(fade - 1);
        }
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
    static bool init{ false };
    static int titleTick{ 0 };
    if (!init)
    {
        SDL_Log("Title");
        CreateMoveTargetObject("titleLogo", "Data/title.png", new Pos[3]{ {-256,0},
            {0,0},{10,0} }, MoveType::EXPONENT);
        auto startButton = CreateMoveTargetObject(
            "titleStartButton", "Data/startbutton.png",
            new Pos[3]{ { 374,-29 }, { 0,1 },{ 374,250 } },
            MoveType::EXPONENT
        );
        startButton->SetTextureFade(60);
        init = true;
    }
    auto titleLogo = objectManager->find("titleLogo");
    auto startButton = objectManager->find("titleStartButton");
    titleLogo->SetMoveVelocity({titleTick,0});
    startButton->SetMoveVelocity({0,titleTick });
    ++titleTick;


}
void Game::update()
{
    float deltaTime = SDL_GetTicks() - curTick / 1000.0f;
    if (deltaTime > 0.05f)
    {
        deltaTime = 0.05f;
    }
    curTick = SDL_GetTicks();
    for (auto object : objectManager->objects())
    {
        object->Move(deltaTime);
    }
}

std::shared_ptr<GameObject> Game::CreateObject(
const std::string& objectName, const std::string&
fileName,Pos* pos,MoveType mType)
// pos[0] - initPos pos[1] - velocity
{
    auto object = std::make_shared<GameObject>(objectName);
    objectManager->AddObject(object);
    object->LoadImage(renderer, fileName);
    object->InitMove(pos[0], pos[1], MoveType::EXPONENT);
    return object;
}

std::shared_ptr<GameObject>  Game::CreateMoveTargetObject(const std::string& objectName, const std::string&
    fileName, Pos* pos, MoveType mType)
    // pos[0] - initPos pos[1] - velocity pos[2] - moveTargetPos
{
    auto object = CreateObject(objectName, fileName, new Pos[2]{ pos[0],pos[1] }, mType);
    Pos targetPos = pos[2];
    object->InitMove(pos[0], pos[1], MoveType::EXPONENT, &targetPos);
    return object;
}
