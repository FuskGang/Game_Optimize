#include "game.h"

static void update(void);
static void update_tank(void);
static void draw(void);
static void init_player(void);
static void draw_player1(void);
static float calculate_delta_time(void);
static void draw_stats(void);
static void do_input(void);
static void update_bullet(void);
static void draw_bullet(Tank *player);

static Tank *player1;
static float delta_time;

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
    player1->size.y = 0;
    player1->size.w = 60;
    player1->size.h = 18;

    player1->muzzle.length = 30;
    player1->muzzle.thickness = 3;
    player1->muzzle.degrees = 0;

    player1->is_shooting = 0;

    SDL_Surface *surface = load_surface("assets/Tank.png");
    SDL_SetSurfaceColorMod(surface, player1->color.r, player1->color.g, player1->color.b);
    player1->texture = to_texture(surface, TRUE);

    surface = load_surface("assets/Bullet2.png");
    SDL_SetSurfaceColorMod(surface, 255, 0, 0);
    player1->bullet.texture = to_texture(surface, TRUE);
    player1->bullet.damage = 10;
    player1->bullet.position.w = 10;
    player1->bullet.position.h = 10;

    int is_ground = 0;
    while (!is_ground)
    {
        for (int i = player1->size.x; i < player1->size.x + player1->size.w; i++)
        {
            if (pixel_map[i][player1->size.y + player1->size.h + 1] == 1) 
            {
                is_ground = 1;
                player1->first_base_pixel.x = i;
                player1->first_base_pixel.y = player1->size.y + player1->size.h;
                player1->second_base_pixel.x = player1->size.x + player1->size.w - (i - player1->size.x);

                for (int j = player1->size.y + player1->size.h; j < player1->size.y + player1->size.h + player1->size.w * 2; j++)
                {
                    if (pixel_map[player1->second_base_pixel.x][j] == 1) 
                    {
                        player1->second_base_pixel.y = j;
                        player1->size.y += abs(player1->second_base_pixel.y - player1->first_base_pixel.y) / 2;
                        break;
                    }
                }
                break;
            }
        }
        player1->size.y += 1;
    }

    player1->degrees = get_angle(player1->first_base_pixel.x, player1->first_base_pixel.y, player1->second_base_pixel.x, player1->second_base_pixel.y) % 180 - 90;
}

static void do_input(void)
{
    if (app.keyboard[SDL_SCANCODE_ESCAPE])
    {
        app.keyboard[SDL_SCANCODE_ESCAPE] = 0;
        init_menu();
    }

    if (player1->is_shooting)
    {
        return;
    }

    if (app.keyboard[SDL_SCANCODE_SPACE])
    {
        player1->is_shooting = 1;
        player1->bullet.angle = player1->muzzle.angle;
        player1->shoot_time = SDL_GetTicks();
        return;
    }

    if (app.keyboard[SDL_SCANCODE_RIGHT])
    {
        player1->muzzle.degrees += 1;

        if (player1->muzzle.degrees > 359)
        {
            player1->muzzle.degrees -= 360;
        }
    }

    if (app.keyboard[SDL_SCANCODE_LEFT])
    {
        player1->muzzle.degrees -= 1;

        if (player1->muzzle.degrees < 0)
        {
            player1->muzzle.degrees += 360;
        }
    }

    if (app.keyboard[SDL_SCANCODE_UP])
    {
        if (player1->power < 100)
        {
            player1->power += 1;
        }
    }

    if (app.keyboard[SDL_SCANCODE_DOWN])
    {
        if (player1->power > 1)
        {
            player1->power -= 1;
        }
    }
}

static void update(void)
{
    delta_time = calculate_delta_time();
    do_input();
    update_tank();
}

static void update_tank(void)
{
    if (player1->is_shooting)
    {
        update_bullet();
    }
    else
    {
        player1->bullet.position.x = player1->muzzle.end_x;
        player1->bullet.position.y = player1->muzzle.end_y;
    }

    player1->muzzle.start_x = player1->size.x + player1->size.w / 2;
    player1->muzzle.start_y = player1->size.y + player1->size.h / 4;
    player1->muzzle.angle = (player1->muzzle.degrees - 90) * M_PI / 180;
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
    blit_rotated(player1->texture, player1->size, FALSE, player1->degrees);
    thickLineRGBA(app.renderer,
                  player1->muzzle.start_x,
                  player1->muzzle.start_y,
                  player1->muzzle.end_x,
                  player1->muzzle.end_y,
                  player1->muzzle.thickness,
                  150, 150, 150, 255);

    if (player1->is_shooting)
    {
        draw_bullet(player1);
    }
}

static float calculate_delta_time(void)
{
    static unsigned int last_frame = 0;
    unsigned int current_frame = SDL_GetTicks();
    delta_time = (float)(current_frame - last_frame) / 1000.0f;
    last_frame = current_frame;
    return delta_time;
}

static void draw_stats(void)
{
    char angle[100], power[100];
    sprintf(angle, "Current angle: %d", player1->muzzle.degrees % 360);
    sprintf(power, "Current power: %d", player1->power);
    draw_text(angle, 50, 50, 255, 255, 255);
    draw_text(power, 50, 80, 255, 255, 255);
}

static void update_bullet(void)
{
    if (player1->bullet.position.x < 0 || player1->bullet.position.x > SCREEN_WIDTH)
    {
        player1->is_shooting = 0;
        return;
    }

    if (player1->bullet.position.y >= 0 &&
        player1->bullet.position.y < SCREEN_HEIGHT &&
        pixel_map[player1->bullet.position.x][player1->bullet.position.y + player1->bullet.position.h] == 1)
    {
        player1->is_shooting = 0;
        return;
    }

    double speed_up = 3;
    double t = speed_up * (SDL_GetTicks() - player1->shoot_time) / 1000.0;
    double x = speed_up * player1->power * t * cos(player1->muzzle.angle);
    double y = speed_up * player1->power * t * sin(player1->muzzle.angle) + speed_up * 0.5 * G * t * t;

    player1->bullet.position.x = player1->muzzle.end_x + x;
    player1->bullet.position.y = player1->muzzle.end_y + y;
}

static void draw_bullet(Tank *player)
{
    blit(player->bullet.texture, &player->bullet.position, 1);
}