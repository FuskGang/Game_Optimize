#include "game.h"

static void update(void);
static void update_tank(void);
static void draw(void);
static void init_player(void);
static void draw_player1(void);
static float calculate_delta_time(void);
static void draw_stats(void);

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
    player1->angle = 60;
    player1->power = 60;
    player1->size.x = 100;
    player1->size.y = 100;
    player1->size.w = 60;
    player1->size.h = 18;
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

    if (app.keyboard[SDL_SCANCODE_UP])
    {
        player1->angle += 1;
        
        if (player1->angle > 359)
        {
            player1->angle = 0;
        }
    }

    if (app.keyboard[SDL_SCANCODE_DOWN])
    {
        player1->angle -= 1;
        
        if (player1->angle < 0)
        {
            player1->angle = 359;
        }
    }
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

    int startX = player1->size.x + player1->size.w / 2;
    int startY = player1->size.y + player1->size.h / 4;

    int length = 30;

    double angle = player1->angle * M_PI / 180; // Convert degrees to radians
    int endX = startX + length * cos(angle);
    int endY = startY + length * sin(angle);

    thickLineRGBA(app.renderer, startX, startY, endX, endY, 3, 150, 150, 150, 255);
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
    sprintf(info, "Current angle: %d", player1->angle);
    draw_text(info, 50, 50, 255, 255, 255);
}