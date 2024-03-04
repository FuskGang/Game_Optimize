#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "constants.h"

SDL_Window *window;
SDL_Renderer *renderer;

SDL_Texture *alien;
SDL_Rect sourceRect;
SDL_Rect destinationRect;

char is_game_running = TRUE;

int initialize()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) >= 0)
    {
        window = SDL_CreateWindow("Tanks",
                                  SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                  640, 480,
                                  0);

        if (window != NULL)
        {
            renderer = SDL_CreateRenderer(window, -1, 0);
        }
    }
    else
    {
        return -1;
    }

    SDL_Surface *image_surface = IMG_Load("assets/alien.png");
    alien = SDL_CreateTextureFromSurface(renderer, image_surface);
    SDL_FreeSurface(image_surface);

    SDL_QueryTexture(alien, NULL, NULL, &sourceRect.w, &sourceRect.h);
    destinationRect.x = sourceRect.x = 0;
    destinationRect.y = sourceRect.y = 0;
    destinationRect.w = sourceRect.w;
    destinationRect.h = sourceRect.h;

    return 0;
}

void handle_input()
{
    SDL_Event event;
    if (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            is_game_running = FALSE;
            break;
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE)
                is_game_running = FALSE;
            break;
        default:
            break;
        }
    }
}

void update()
{
}

void draw()
{
    SDL_SetRenderDrawColor(renderer, 0, 50, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, alien, &sourceRect, &destinationRect);
    SDL_RenderPresent(renderer);
    // SDL_Delay(100);
}

void clean()
{
    SDL_DestroyTexture(alien);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    IMG_Quit();
    SDL_Quit();
}

int main(int argc, char *args[])
{
    is_game_running = !initialize();

    while (is_game_running)
    {
        handle_input();
        update();
        draw();
    }

    clean();

    return 0;
}
