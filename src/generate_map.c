#include "generate_map.h"

#define SKY_RGBA 39, 80, 245, 1
#define GRASS_RGBA 39, 136, 48, 1
#define GEN_FUNC_A -6.0f

int pixel_map[SCREEN_WIDTH][SCREEN_HEIGHT];

static void generate_pixel_map();
static void draw_pixel_map(void);
static int get_noise(double x, double a);
static double noise_function(double x, double a);
static void update(void);
static void draw(void);

void init_map(void)
{
    size_t size = sizeof(pixel_map);
    memset(pixel_map, 0, size);
    generate_pixel_map();

    app.delegate.update = &update;
    app.delegate.draw = &draw;
}

static void update(void)
{
    if (app.keyboard[SDL_SCANCODE_ESCAPE])
    {
        app.keyboard[SDL_SCANCODE_ESCAPE] = 0;
        init_menu();
    }
}

static void draw(void)
{
    draw_pixel_map();
}


static void generate_pixel_map()
{
    srand(time(NULL));

    double x = GEN_FUNC_A;
    double x_offset = fabs(GEN_FUNC_A) * 2 / (double)SCREEN_WIDTH;

    double a = (rand() % 200) / 100.0f; // 0.0 - 2.0

    for (int pixel_x = 0; pixel_x < SCREEN_WIDTH; pixel_x++)
    {
        x += x_offset;
        int pixel_y = get_noise(x, a);

        printf("%d\n", pixel_y);

        while (pixel_y < SCREEN_HEIGHT)
        {
            pixel_map[pixel_x][pixel_y++] = 1;
        }
    }
}

static int get_noise(double x, double a)
{
    double tmp_noise = noise_function(x, a);
    int level_noise = (int)tmp_noise;

    return level_noise;
}

static double noise_function(double x, double a)
{
    double y = fabs(sin(1 + cos(x) - cos(a) + (a / cos(5 * a))) + (x * a) / 3);
    double fix_y = y * 100.0f + (SCREEN_HEIGHT) * 0.32f;

    return fix_y;
}

static void draw_pixel_map(void)
{
    SDL_Rect r = {.w = 1, .h = 1};
    SDL_SetRenderDrawColor(app.renderer, GRASS_RGBA);
    for (int pixel_x = 0; pixel_x < SCREEN_WIDTH; pixel_x++)
    {
        for (int pixel_y = 0; pixel_y < SCREEN_HEIGHT; pixel_y++)
        {
            if (pixel_map[pixel_x][pixel_y] == 1)
            {
                r.x = pixel_x;
                r.y = pixel_y;
                SDL_RenderFillRect(app.renderer, &r);
            }
        }
    }
}
