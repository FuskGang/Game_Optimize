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

void blit(SDL_Texture *texture, int x, int y, int center)
{
    SDL_Rect dest;

    dest.x = x;
    dest.y = y;
    SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);

    if (center)
	{
		dest.x -= dest.w / 2;
		dest.y -= dest.h / 2;
	}

    SDL_RenderCopy(app.renderer, texture, NULL, &dest);
}
