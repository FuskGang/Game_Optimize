#include "draw.h"

void prepare_frame(void)
{
    SDL_SetRenderDrawColor(app.renderer, 32, 32, 32, 255);
    SDL_RenderClear(app.renderer);
}

void present_frame(void)
{
    SDL_RenderPresent(app.renderer);
}

SDL_Texture *load_texture(char *filename)
{
    SDL_Texture *texture;

    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s", filename);

    texture = IMG_LoadTexture(app.renderer, filename);

    return texture;
}

SDL_Surface *load_surface(char *filename)
{
    SDL_Surface *surface;

    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s", filename);

    surface = IMG_Load(filename);

    return surface;
}

void blit(SDL_Texture *texture, SDL_Rect dest, int center)
{
    if (center)
    {
        dest.x -= dest.w / 2;
        dest.y -= dest.h / 2;
    }

    SDL_RenderCopy(app.renderer, texture, NULL, &dest);
}

void blit_rotated(SDL_Texture *texture, SDL_Rect dest, int center, float angle)
{
    if (center)
    {
        dest.x -= (dest.w / 2);
        dest.y -= (dest.h / 2);
    }

    SDL_RenderCopyEx(app.renderer, texture, NULL, &dest, angle, NULL, SDL_FLIP_NONE);
}