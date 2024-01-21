#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_ttf.h>
#include "SDL2/SDL_image.h"
#include "../MobileEntity/Entity.h"
#include "../Player/Player.h"

#include "../MobileEntity/EnemyEntity.h"
#include "../Enemy/Enemy.h"
#include "../Gun/Bullet.h"

#include "../Arena/Arena.h"
#include "../EnemyFactory/EnemyFactory.h"
#include "../Menu/MenuUI.h"
#include "../StateMachine/State.h"

#define WIDTH 1000
#define HEIGHT 1000

class Game
{
private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    State *tate;

public:
    Game()
    {
        try
        {
            SDL_Init(SDL_INIT_VIDEO);
            window = SDL_CreateWindow("#DROPOUT", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
            std::cerr << "SDL_Init failed" << '\n';
        }
        SetState(tate, renderer, GameMenu_N);
    }

    void Run()
    {
        SDL_Event event;
        bool quit = false;

        Uint32 start = SDL_GetTicks();
        float deltaTime = 0.1f;
        while (!quit)
        {
            ChangeState(tate, renderer);
            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_QUIT)
                    quit = true;
                try
                {
                    tate->GatherInput(event, deltaTime);
                }
                catch (const std::exception &e)
                {
                    std::cerr << e.what() << '\n';
                }
            }
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);

            tate->Render(deltaTime);

            Uint32 endTime = SDL_GetTicks();
            deltaTime = (endTime - start) / 10.0f;
            start = endTime;

            SDL_RenderPresent(renderer);
            SDL_Delay(10);
        }

        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
};
