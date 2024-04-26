#include "game.h"

static void update(void);
static void draw(void);
static void init_player(void);
static void get_player(Tank *, char *,
                       int, int, int,
                       int, int, int, int,
                       int, int, int,
                       char *, int, int, int, int, int, int,
                       int, char *);
static void drop_player(Tank *);
static void draw_player(Tank *);
static float calculate_delta_time(void);
static void draw_stats(Tank *);
static void do_input(Tank *);
static void update_bullet(Tank *);
static void update_tank(Tank *);
static void draw_bullet(Tank *);
static void swap_player(void);

static Tank *player1;
static Tank *player2;
static Tank *curr_player;
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
    player2 = malloc(sizeof(Tank));
    curr_player = player1;

    get_player(player1, "Test",
               255, 255, 0,
               100, 0, 60, 18,
               30, 3, 0,
               "assets/Bullet2.png", 10, 10, 10, 255, 0, 0,
               60, "assets/Tank.png");

    get_player(player2, "Test2",
               255, 0, 255,
               SCREEN_WIDTH - 100, 0, 60, 18,
               30, 3, 0,
               "assets/Bullet2.png", 10, 10, 10, 255, 0, 0,
               60, "assets/Tank.png");

    drop_player(player1);
    drop_player(player2);
}

static void get_player(Tank *player, char *name, 
                       int player_r, int player_g, int player_b, 
                       int size_x, int size_y, int size_w, int size_h, 
                       int muzzle_length, int muzzle_thickness, int muzzle_degrees,
                       char *bullet_texture_path, int damage, int bullet_w, int bullet_h, int bullet_r, int bullet_g, int bullet_b,
                       int power, char *player_texture_path)
{
    SDL_Surface *bullet_surface = load_surface(bullet_texture_path);
    SDL_SetSurfaceColorMod(bullet_surface, bullet_r, bullet_g, bullet_b);
    SDL_Texture *bullet_texture = to_texture(bullet_surface, TRUE);


    SDL_Surface *player_surface = load_surface(player_texture_path);
    SDL_SetSurfaceColorMod(player_surface, player_r, player_g, player_b);
    SDL_Texture *player_texture = to_texture(player_surface, TRUE);

    strcpy(player->player_name, name);

    player->color.r = player_r;
    player->color.g = player_g;
    player->color.b = player_b;

    player->size.x = size_x;
    player->size.y = size_y;
    player->size.w = size_w;
    player->size.h = size_h;

    player->muzzle.length = muzzle_length;
    player->muzzle.thickness = muzzle_thickness;
    player->muzzle.degrees = muzzle_degrees;

    player->bullet.texture = bullet_texture;
    player->bullet.damage = damage;
    player->bullet.position.w = bullet_w;
    player->bullet.position.h = bullet_h;

    player->power = power;
    player->texture = player_texture;
    player->is_shooting = 0;
}

static void drop_player(Tank *player)
{
    int is_ground = 0;
    while (!is_ground)
    {
        for (int i = player->size.x; i < player->size.x + player->size.w; i++)
        {
            if (pixel_map[i][player->size.y + player->size.h + 1] == 1)
            {
                is_ground = 1;
                player->first_base_pixel.x = i;
                player->first_base_pixel.y = player->size.y + player->size.h;
                player->second_base_pixel.x = player->size.x + player->size.w - (i - player->size.x);

                for (int j = player->size.y + player->size.h; j < player->size.y + player->size.h + player->size.w * 2; j++)
                {
                    if (pixel_map[player->second_base_pixel.x][j] == 1)
                    {
                        player->second_base_pixel.y = j;
                        player->size.y += abs(player->second_base_pixel.y - player->first_base_pixel.y) / 2;
                        break;
                    }
                }
                break;
            }
        }
        player->size.y += 1;
    }

    player->degrees = get_angle(player->first_base_pixel.x, player->first_base_pixel.y, player->second_base_pixel.x, player->second_base_pixel.y) % 180 - 90;
}

static void do_input(Tank *player)
{
    if (app.keyboard[SDL_SCANCODE_ESCAPE])
    {
        app.keyboard[SDL_SCANCODE_ESCAPE] = 0;
        init_menu();
    }

    if (player->is_shooting)
    {
        return;
    }

    if (app.keyboard[SDL_SCANCODE_SPACE])
    {
        player->is_shooting = 1;
        player->bullet.angle = player->muzzle.angle;
        player->shoot_time = SDL_GetTicks();

        return;
    }

    if (app.keyboard[SDL_SCANCODE_RIGHT])
    {
        player->muzzle.degrees += 1;

        if (player->muzzle.degrees > 359)
        {
            player->muzzle.degrees -= 360;
        }
    }

    if (app.keyboard[SDL_SCANCODE_LEFT])
    {
        player->muzzle.degrees -= 1;

        if (player->muzzle.degrees < 0)
        {
            player->muzzle.degrees += 360;
        }
    }

    if (app.keyboard[SDL_SCANCODE_UP])
    {
        if (player->power < 100)
        {
            player->power += 1;
        }
    }

    if (app.keyboard[SDL_SCANCODE_DOWN])
    {
        if (player->power > 1)
        {
            player->power -= 1;
        }
    }
}

static void update(void)
{
    delta_time = calculate_delta_time();

    do_input(curr_player);

    update_tank(player1);
    update_tank(player2);
}

static void update_tank(Tank *player)
{
    if (player->is_shooting)
    {
        update_bullet(player);
    }
    else
    {
        player->bullet.position.x = player->muzzle.end_x;
        player->bullet.position.y = player->muzzle.end_y;
    }

    player->muzzle.start_x = player->size.x + player->size.w / 2;
    player->muzzle.start_y = player->size.y + player->size.h / 4;
    player->muzzle.angle = (player->muzzle.degrees - 90) * M_PI / 180;
    player->muzzle.end_x = player->muzzle.start_x + player->muzzle.length * cos(player->muzzle.angle);
    player->muzzle.end_y = player->muzzle.start_y + player->muzzle.length * sin(player->muzzle.angle);
}

static void draw(void)
{
    draw_pixel_map();
    draw_player(player1);
    draw_player(player2);
    draw_stats(curr_player);
}

static void draw_player(Tank *player)
{
    blit_rotated(player->texture, player->size, FALSE, player->degrees);
    thickLineRGBA(app.renderer,
                  player->muzzle.start_x,
                  player->muzzle.start_y,
                  player->muzzle.end_x,
                  player->muzzle.end_y,
                  player->muzzle.thickness,
                  150, 150, 150, 255);

    if (player->is_shooting)
    {
        draw_bullet(player);
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

static void draw_stats(Tank *player)
{
    char angle[100], power[100];
    sprintf(angle, "Current angle: %d", player->muzzle.degrees % 360);
    sprintf(power, "Current power: %d", player->power);
    draw_text(angle, 50, 50, player->color.r, player->color.g, player->color.b);
    draw_text(power, 50, 80, player->color.r, player->color.g, player->color.b);
}

static void update_bullet(Tank *player)
{
    if (player->bullet.position.x < 0 || player->bullet.position.x > SCREEN_WIDTH)
    {
        player->is_shooting = 0;
        swap_player();
        return;
    }

    if (player->bullet.position.y >= 0 &&
        player->bullet.position.y < SCREEN_HEIGHT &&
        pixel_map[player->bullet.position.x][player->bullet.position.y + player->bullet.position.h] == 1)
    {
        player->is_shooting = 0;
        swap_player();
        return;
    }

    double speed_up = 3;
    double t = speed_up * (SDL_GetTicks() - player->shoot_time) / 1000.0;
    double x = speed_up * player->power * t * cos(player->muzzle.angle);
    double y = speed_up * player->power * t * sin(player->muzzle.angle) + speed_up * 0.5 * G * t * t;

    player->bullet.position.x = player->muzzle.end_x + x;
    player->bullet.position.y = player->muzzle.end_y + y;
}

static void draw_bullet(Tank *player)
{
    blit(player->bullet.texture, &player->bullet.position, 1);
}

static void swap_player(void)
{
    if (curr_player == player1)
    {
        curr_player = player2;
    }
    else
    {
        curr_player = player1;
    }
}
