#include "map.h"

#define SKY_RGBA 39, 80, 245, 1
#define GRASS_RGBA 39, 136, 48, 1
#define GEN_FUNC_A -6.0f

static void generate_pixel_map(void);
static int get_noise(double x, double a);
static double noise_function(double x, double a);
static SDL_Texture *get_map_texture(void);

int pixel_map[SCREEN_WIDTH][SCREEN_HEIGHT];
static SDL_Texture *map;

void init_map(void)
{
    size_t size = sizeof(pixel_map);
    memset(pixel_map, 0, size);
    generate_pixel_map();
}

void generate_pixel_map()
{
    srand(time(NULL));

    double x = GEN_FUNC_A;
    double x_offset = fabs(GEN_FUNC_A) * 2 / (double)SCREEN_WIDTH;

    double a = (rand() % 200) / 100.0f; // 0.0 - 2.0

    for (int pixel_x = 0; pixel_x < SCREEN_WIDTH; pixel_x++)
    {
        x += x_offset;
        int pixel_y = get_noise(x, a);

        while (pixel_y < SCREEN_HEIGHT)
        {
            pixel_map[pixel_x][pixel_y++] = 1;
        }
    }

    set_map();
}

static int get_noise(double x, double a)
{
    double tmp_noise = noise_function(x, a);
    int level_noise = (int)tmp_noise;

    return level_noise;
}

void set_map(void)
{
    SDL_DestroyTexture(map);
    map = get_map_texture();
}

static SDL_Texture *get_map_texture(void)
{
    SDL_Surface *surface = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0, 0, 0, 0);
    if (surface == NULL)
    {
        return NULL;
    }

    for (int y = 0; y < SCREEN_HEIGHT; y++)
    {
        for (int x = 0; x < SCREEN_WIDTH; x++)
        {
            Uint32 pixelColor = (pixel_map[x][y] == 0) ? SDL_MapRGBA(surface->format, SKY_RGBA) : SDL_MapRGBA(surface->format, GRASS_RGBA);
            SDL_Rect pixelRect = {x, y, 1, 1};
            SDL_FillRect(surface, &pixelRect, pixelColor);
        }
    }

    SDL_Texture *texture = to_texture(surface, TRUE);

    return texture;
}

static double noise_function(double x, double a)
{
    double y = fabs(sin(1 + cos(x) - cos(a) + (a / cos(5 * a))) + (x * a) / 3);
    double fix_y = y * 100.0f + (SCREEN_HEIGHT) * 0.32f;

    return fix_y;
}

void draw_pixel_map(void)
{
    blit(map, NULL, FALSE);
}
