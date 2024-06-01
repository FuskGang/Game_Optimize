#include "draw.h"

#define MAX_TEXTURES 100

typedef struct
{
    char *filename;
    SDL_Texture *texture;
} CachedTexture;

static CachedTexture texture_cache[MAX_TEXTURES];
static int num_textures = 0;

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
    for (int i = 0; i < num_textures; i++)
    {
        if (strcmp(texture_cache[i].filename, filename) == 0)
        {
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading from cache %s", filename);
            return texture_cache[i].texture;
        }
    }

    SDL_Texture *texture;

    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s", filename);

    texture = IMG_LoadTexture(app.renderer, filename);

    if (num_textures < MAX_TEXTURES)
    {
        texture_cache[num_textures].filename = filename;
        texture_cache[num_textures].texture = texture;
        num_textures++;
    }

    return texture;
}

SDL_Surface *load_surface(char *filename)
{
    SDL_Surface *surface;

    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s", filename);

    surface = IMG_Load(filename);

    return surface;
}

void blit(SDL_Texture *texture, const SDL_Rect *dest, int center)
{
    SDL_Rect *dest_copy;
    dest_copy = (SDL_Rect *)malloc(sizeof(SDL_Rect));

    if (dest != NULL)
    {
        *dest_copy = *dest;
        if (center)
        {
            dest_copy->x -= dest_copy->w / 2;
            dest_copy->y -= dest_copy->h / 2;
        }
    }
    else
    {
        free(dest_copy);
        dest_copy = NULL;
    }

    SDL_RenderCopy(app.renderer, texture, NULL, dest_copy);
}

void blit_rotated(SDL_Texture *texture, const SDL_Rect *dest, int center, float angle)
{
    SDL_Rect *dest_copy;
    dest_copy = (SDL_Rect *)malloc(sizeof(SDL_Rect));

    if (dest != NULL)
    {
        *dest_copy = *dest;
        if (center)
        {
            dest_copy->x -= dest_copy->w / 2;
            dest_copy->y -= dest_copy->h / 2;
        }
    }
    else
    {
        free(dest_copy);
        dest_copy = NULL;
    }

    SDL_RenderCopyEx(app.renderer, texture, NULL, dest_copy, angle, NULL, SDL_FLIP_NONE);
}