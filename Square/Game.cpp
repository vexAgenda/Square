
#include "Game.h"

Game::Game() :
    objectManager{ new ObjectManager{} }
{

}

bool Game::init()
{
    int sdlInit = SDL_Init(SDL_INIT_EVERYTHING);
    if (sdlInit != 0)
    {
        SDL_Log("init failed : %s", SDL_GetError());
        return false;
    }
    window = SDL_CreateWindow("Square ManiaQ",
        100, 100,
        512, 768, SDL_WINDOW_OPENGL);

    if (!window)
    {
        SDL_Log("failed to create window : %s", SDL_GetError());
        return false;
    }

    renderer = SDL_CreateRenderer(window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer)
    {
        SDL_Log("failed to create renderer: %s", SDL_GetError());
        return false;
    }
    SDL_Surface* icon= IMG_Load("Data/icon.png");
    SDL_SetWindowIcon(window, icon);
    

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
    {
        SDL_Log("failed to init image : %s", SDL_GetError());
        return false;
    }

    // Set Background to white
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    return true;
}

void Game::splash()
{
    SDL_Log("Splash Screen");
    static Uint32 splashTick{ 0 };
    static Uint32 splashcurTick{ 0 };
    auto splash{ std::make_shared<GameObject>()};
    splash->LoadImage(renderer,"Data/splash.png");
    splash->setPos( 512 / 2, 768 / 2 );
    objectManager->AddObject(splash);
    while (splashTick < 60)
    {
        render(splashTick);
        ++splashTick;
        SDL_Delay(25);
    }
    objectManager->DeleteObject(splash);
    SDL_Log("Splash End");
}

void Game::run()
{
    while (bRun)
    {
        input();
        update();
        render(Fade);
    }
}

void Game::quit()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

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
        SDL_SetTextureAlphaMod(object->texture(), 255 - 4 * fade);
        if (SDL_RenderCopy(renderer, object->texture(),
            &rect, &posRect) != 0)
        {
            SDL_Log("Render Failed: %s",SDL_GetError());
        }
    }
    SDL_RenderPresent(renderer);

    /*SDL_Surface* surface = IMG_Load("Data/splash.png");
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer,
    surface);

    SDL_Rect src;
    SDL_Rect dst;

    src.x = 0;
    src.y = 0;
    src.w = 256;
    src.h = 128;

    dst.x = 512 / 2;
    dst.y = 768 / 2;
    dst.w = src.w;
    dst.h = src.h;
    SDL_RenderClear(renderer);
    SDL_QueryTexture(texture, NULL, NULL, &src.w, &src.h);
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    SDL_SetTextureAlphaMod(texture, 128);
    SDL_RenderCopy(renderer, texture, &src, &dst);
    SDL_RenderPresent(renderer);*/

}
void Game::update()
{
}

