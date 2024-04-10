#include "game.h"

static void update(void);
static void update_tank(void);
static void draw(void);
static void init_player();
static void draw_player1(void);
static float calculate_delta_time(void);

static Tank *player1;

void init_game(void)
{
    init_map();
    init_player();

    app.delegate.update = update;
    app.delegate.draw = draw;
}

static void init_player()
{
    player1 = malloc(sizeof(Tank));
    strcpy(player1->player_name, "Test");
    player1->color.r = 255;
    player1->color.g = 0;
    player1->color.b = 0;
    player1->angle = 60;
    player1->power = 60;
    player1->x = 100;
    player1->y = 100;
    SDL_Surface *surface = load_surface("assets/Tank.png");
    SDL_SetSurfaceColorMod(surface, player1->color.r, player1->color.g, player1->color.b);
    player1->texture = to_texture(surface, TRUE);
}

static void update(void)
{
    update_tank();

    if (app.keyboard[SDL_SCANCODE_ESCAPE])
    {
        app.keyboard[SDL_SCANCODE_ESCAPE] = 0;
        init_menu();
    }
}

static void update_tank(void)
{
    float deltaTime = calculate_delta_time();

    int textureWidth, textureHeight;
    SDL_QueryTexture(player1->texture, NULL, NULL, &textureWidth, &textureHeight);
    if (pixel_map[player1->x][player1->y + textureHeight] != 1 && pixel_map[player1->x + textureWidth][player1->y + textureHeight] != 1)
    {
        player1->y += 200 * deltaTime;
    }
}

static void draw(void)
{
    draw_pixel_map();
    draw_player1();
}

static void draw_player1(void)
{
    blit(player1->texture, player1->x, player1->y, FALSE);
}

static float calculate_delta_time(void)
{
    static unsigned int lastFrame = 0;
    unsigned int currentFrame = SDL_GetTicks();
    float deltaTime = (float)(currentFrame - lastFrame) / 1000.0f;
    lastFrame = currentFrame;
    return deltaTime;
}