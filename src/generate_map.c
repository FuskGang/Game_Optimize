#include "generate_map.h"

#define SKY_RGBA 39, 80, 245, 1
#define GRASS_RGBA 39, 136, 48, 1
#define GEN_FUNC_A -6.0f
#define WHITE 255, 255, 255

int pixel_map[SCREEN_WIDTH][SCREEN_HEIGHT];
static SDL_Texture *bulletTexture;
static Bullet bullet;

static void update(void);
static void draw(void);

void init_map(void)
{
    size_t size = sizeof(pixel_map);
    memset(pixel_map, 0, size);
    generate_pixel_map();

    app.delegate.update = &update;
    app.delegate.draw = &draw;

    bulletTexture = load_texture("assets/bullet2.png");
}

static void update(void)
{
    if (app.keyboard[SDL_SCANCODE_ESCAPE])
    {
        app.keyboard[SDL_SCANCODE_ESCAPE] = 0;
        init_menu();
    }

    if (app.keyboard[SDL_SCANCODE_Z] && app.bullet.was_fire == 0)
    {
        app.bullet.velocity = app.bullet.velocity - 10 > 0 ? app.bullet.velocity - 10 : 0;
    }

    if (app.keyboard[SDL_SCANCODE_X] && app.bullet.was_fire == 0)
    {
        app.bullet.velocity = app.bullet.velocity + 10 < 100 ? app.bullet.velocity + 10 : 100;
    }

    if (app.keyboard[SDL_SCANCODE_LCTRL] && app.bullet.was_fire == 0)
    {
        app.bullet.angle = app.bullet.angle - 0.01;
    }

    if (app.keyboard[SDL_SCANCODE_LSHIFT] && app.bullet.was_fire == 0)
    {
        app.bullet.angle = app.bullet.angle + 0.01;
    }

    if (app.keyboard[SDL_SCANCODE_SPACE] && app.bullet.was_fire == 0)
    {

        fire(&bullet, app.mouse.x, app.mouse.y, -(3.1415926 / 3), app.bullet.velocity);
        app.bullet.was_fire = 1;
    }

    if (app.bullet.was_fire)
    {
        update_pos_b(&bullet, 0.016f, app.bullet.angle);
        app.bullet.was_fire = render_b(&bullet);
    }
}

static void draw(void)
{
    draw_pixel_map();
    draw_shoot_info();
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

        printf("%d\n", pixel_y);

        while (pixel_y < SCREEN_HEIGHT)
        {
            pixel_map[pixel_x][pixel_y++] = 1;
        }
    }
}

int get_noise(double x, double a)
{
    double tmp_noise = noise_function(x, a);
    int level_noise = (int)tmp_noise;

    return level_noise;
}

double noise_function(double x, double a)
{
    double y = fabs(sin(1 + cos(x) - cos(a) + (a / cos(5 * a))) + (x * a) / 3);
    double fix_y = y * 100.0f + (SCREEN_HEIGHT) * 0.32f;

    return fix_y;
}

void draw_pixel_map(void)
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

void draw_shoot_info()
{
    char info[100];

    sprintf(info, "Your velocity: %d", (int)app.bullet.velocity);
    draw_text(info, 10, 10, WHITE);

    sprintf(info, "Your angle: %.1f", app.bullet.angle);
    draw_text(info, 10, 50, WHITE);
}

void fire(Bullet *b, float x, float y, float angle, float velocity)
{
    b->x = x;
    b->y = y;
    b->angle = angle;
    b->velocity = velocity;
}

void update_pos_b(Bullet *b, float time_wased, float angle)
{
    float dx = b->velocity * cos(b->angle) * time_wased;
    float dy = b->velocity * sin(b->angle) * time_wased - 0.5f * time_wased * time_wased;

    b->was_x = b->x;
    b->was_y = b->y;
    b->x += dx;
    b->y += dy;
    b->angle += angle * time_wased;

    app.bullet.was_fire = !(b->x >= 1280 || b->y >= 720);
}

int render_b(Bullet *b)
{
    blit(bulletTexture, b->x, b->y, 1);
    return 0;
    // dst->x = b->was_x;
    // dst->y = b->was_y;

    // SDL_RenderFillRect(renderer, dst);

    // dst->x = b->x;
    // dst->y = b->y;

    // if (dst->x >= 640 || dst->y >= 640)
    // {
    //     return false;
    // }

    // SDL_RenderCopy(renderer, texture, NULL, dst);

    // return true;
}