#include "game.h"

static void update(void);
static void update_tank(void);
static void draw(void);
static void init_player(void);
static void draw_player1(void);
static float calculate_delta_time(void);
static void draw_stats(void);
static void do_input(void);

static Tank *player1;

void init_game(void)
{
    init_map();
    init_player();

    app.delegate.update = update;
    app.delegate.draw = draw;
}

static void init_player(void)
{
    player1 = malloc(sizeof(Tank));
    strcpy(player1->player_name, "Test");

    player1->color.r = 255;
    player1->color.g = 255;
    player1->color.b = 0;
    player1->power = 60;
    player1->size.x = 100;
    player1->size.y = 100;
    player1->size.w = 60;
    player1->size.h = 18;

    player1->muzzle.length = 30;
    player1->muzzle.thickness = 3;
    player1->muzzle.degrees = 0;

    SDL_Surface *surface = load_surface("assets/Tank.png");
    SDL_SetSurfaceColorMod(surface, player1->color.r, player1->color.g, player1->color.b);
    player1->texture = to_texture(surface, TRUE);
}

static void do_input(void)
{
    if (app.keyboard[SDL_SCANCODE_ESCAPE])
    {
        app.keyboard[SDL_SCANCODE_ESCAPE] = 0;
        init_menu();
    }

    if (app.keyboard[SDL_SCANCODE_UP])
    {
        player1->muzzle.degrees += 1;

        if (player1->muzzle.degrees > 419)
        {
            player1->muzzle.degrees -= 360;
        }
    }

    if (app.keyboard[SDL_SCANCODE_DOWN])
    {
        player1->muzzle.degrees -= 1;

        if (player1->muzzle.degrees < 90)
        {
            player1->muzzle.degrees += 360;
        }
    }
}

static void update(void)
{
    do_input();
    update_tank();
}

static void update_tank(void)
{
    float deltaTime = calculate_delta_time();

    int textureWidth, textureHeight;
    SDL_QueryTexture(player1->texture, NULL, NULL, &textureWidth, &textureHeight);
    if (pixel_map[player1->size.x][player1->size.y + textureHeight] != 1 && pixel_map[player1->size.x + textureWidth][player1->size.y + textureHeight] != 1)
    {
        player1->size.y += 200 * deltaTime;
    }

    player1->muzzle.start_x = player1->size.x + player1->size.w / 2;
    player1->muzzle.start_y = player1->size.y + player1->size.h / 4;
    player1->muzzle.angle = player1->muzzle.degrees * M_PI / 180; // Convert degrees to radians
    player1->muzzle.end_x = player1->muzzle.start_x + player1->muzzle.length * cos(player1->muzzle.angle);
    player1->muzzle.end_y = player1->muzzle.start_y + player1->muzzle.length * sin(player1->muzzle.angle);
}

static void draw(void)
{
    draw_pixel_map();
    draw_player1();
    draw_stats();
}

static void draw_player1(void)
{
    blit(player1->texture, player1->size, FALSE);
    thickLineRGBA(app.renderer,
                  player1->muzzle.start_x,
                  player1->muzzle.start_y,
                  player1->muzzle.end_x,
                  player1->muzzle.end_y,
                  player1->muzzle.thickness,
                  150, 150, 150, 255);
}

static float calculate_delta_time(void)
{
    static unsigned int lastFrame = 0;
    unsigned int currentFrame = SDL_GetTicks();
    float deltaTime = (float)(currentFrame - lastFrame) / 1000.0f;
    lastFrame = currentFrame;
    return deltaTime;
}

static void draw_stats(void)
{
    char info[100];
    sprintf(info, "Current angle: %d", (player1->muzzle.degrees + 90) % 360);
    draw_text(info, 50, 50, 255, 255, 255);
}