
#include "Game.h"
#include "Button.h"

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
        splash = CreateObject<GameObject>("splash", "Data/splash.png", 
            new Vector2F[2]{ { 512 / 2,768 / 2 }, { 0,0 } }, MoveType::SQUARE);
        //set alpha to 240
        splash->InitFade(Fade::FADE_OUT, 60, 4);
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
        e.eid = EID::TITLE_ENTER;
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
        case EID::TITLE_ENTER:
            gameState = GameState::TITLE_ENTER;
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
    case GameState::TITLE_ENTER:
        titleEnter();
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
            bRun = false;
            quit();
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
            renderFadeIn(object);
            break;
        case Fade::FADE_OUT:
            renderFadeOut(object);
            break;
        }
        if (SDL_RenderCopyF(renderer, object->texture(),
            &rect, &posRect) != 0)
        {
            SDL_Log("Render Failed: %s",SDL_GetError());
        }
    }
    SDL_RenderPresent(renderer);

}
void Game::renderFadeOut(std::shared_ptr<GameObject> object)
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
        object->SetCurrentFade(currentFade - 1);
    }
}
void Game::renderFadeIn(std::shared_ptr<GameObject> object)
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
        object->SetCurrentFade(currentFade - 1);
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
        CreateMoveTargetObject<GameObject>("titleLogo", "Data/title.png", new Vector2F[3]{ {-256,0},
            {0,0},{10,0} }, MoveType::SQUARE);
        auto startButton = CreateMoveTargetObject<Button>(
            "titleStartButton", "Data/startbutton.png",
            new Vector2F[3]{ { 374,-29 }, { 0,1 },{ 374,250 } },
            MoveType::SQUARE
        );
        init = true;
    }
    auto titleLogo = objectManager->find("titleLogo");
    auto startButton = objectManager->find("titleStartButton");

    if (titleTick > 150)
    {
        startButton->SetMoveType(MoveType::DEFAULT);
        Event e;
        e.eid = EID::TITLE;
        eventHandler->PushEvent(std::make_shared<Event>( e));
    }
    titleLogo->SetMoveVelocity({ titleTick / 4,0 });
    startButton->SetMoveVelocity({ 0,titleTick / 4 });
    ++titleTick;


}
void Game::title()
{
    for (auto object : objectManager->objects())
    {
        static int buttonMovVel{ 0 };
        if (object == objectManager->find("titleStartButton"))
        {
            object->ClearMoveTarget();
            if (object->isMouseCollide(_mouse))
            {
                object->SetMoveTarget({334, 250});
                object->SetMoveVelocity({ --buttonMovVel,0 });
            }
            else
            {
                object->SetMoveTarget({ 374, 250 });
                object->SetMoveVelocity({ ++buttonMovVel,0 });
            }
            if (std::abs(buttonMovVel) > 50)
                buttonMovVel = 0;
        }
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
    for (auto object : objectManager->objects())
    {
        object->Move(deltaTime);
    }
}

