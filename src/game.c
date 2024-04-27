#include "game.h"

static void update(void);
static void draw(void);
static void init_player(void);
static void get_player(Tank *, char *,
                       int, int, int,
                       int, int, int, int,
                       int, int, int,
                       char *, int, int, int, int, int,
                       int, char *);
static void get_weapon(Weapon *, char *, int, int, int);
static void drop_player(Tank *);
static void draw_player(Tank *);
static float calculate_delta_time(void);
static void draw_stats(Tank *);
static void do_input(Tank *);
static void update_bullet(Tank *, int);
static void update_tank(Tank *);
static void update_hit(Tank *, int);
static void draw_hit(Tank *, int);
static void draw_bullet(Tank *, int);
static void drop_earth(int, int, int);
static collision check_earth_collision(Bullet);
static collision check_tank_collision(Tank *, int, int, int);
static void swap_player(void);

static Tank *player1;
static Tank *player2;
static Tank *curr_player;
static Tank *other_player;
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
    other_player = player2;

    get_player(player1, "Test",
               255, 255, 0,
               100, 0, 60, 18,
               30, 3, 0,
               "assets/Bullet2.png", 10, 10, 255, 0, 0,
               60, "assets/Tank.png");

    get_player(player2, "Test2",
               255, 0, 255,
               SCREEN_WIDTH - 800, 0, 60, 18,
               30, 3, 0,
               "assets/Bullet2.png", 10, 10, 255, 0, 0,
               60, "assets/Tank.png");

    drop_player(player1);
    drop_player(player2);
}

static void get_player(Tank *player, char *name,
                       int player_r, int player_g, int player_b,
                       int size_x, int size_y, int size_w, int size_h,
                       int muzzle_length, int muzzle_thickness, int muzzle_degrees,
                       char *bullet_texture_path, int bullet_w, int bullet_h, int bullet_r, int bullet_g, int bullet_b,
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

    for (int i = 0; i < 3; i++)
    {
        player->bullet[i].texture = bullet_texture;
        player->bullet[i].position.w = bullet_w;
        player->bullet[i].position.h = bullet_h;
        player->bullet[i].is_cur_up = 0;
        player->bullet[i].cur_rad = 0;
        player->bullet[i].is_hit = 0;
        player->bullet[i].is_shoot = 0;
    }

    player->power = power;
    player->texture = player_texture;
    player->points = 0;
    player->was_touch = 0;
    player->was_shoot = 0;

    get_weapon(&player->list_weapon[0], "Default", 10, 50, 1);
    get_weapon(&player->list_weapon[1], "Deagle", 35, 25, 1);
    get_weapon(&player->list_weapon[2], "Minigun", 5, 12, 3);

    player->curr_weapon = player->list_weapon[0];
}

static void get_weapon(Weapon *weapon, char *weapon_name, int damage, int max_radius, int bullet_count)
{
    strcpy(weapon->weapon_name, weapon_name);
    weapon->damage = damage;
    weapon->max_radius = max_radius;
    weapon->bullet_count = bullet_count;
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

    if (player->was_shoot)
    {
        return;
    }

    if (app.keyboard[SDL_SCANCODE_SPACE])
    {
        player->was_shoot = 1;

        for (int curr_bull_ind = 0; curr_bull_ind < player->curr_weapon.bullet_count; curr_bull_ind++)
        {
            player->bullet[curr_bull_ind].is_shoot = 1;
            player->bullet[curr_bull_ind].angle = player->muzzle.angle;
        }

        player->shoot_time = SDL_GetTicks();

        return;
    }

    if (app.keyboard[SDL_SCANCODE_1])
    {
        player->curr_weapon = player->list_weapon[0];
    }

    if (app.keyboard[SDL_SCANCODE_2])
    {
        player->curr_weapon = player->list_weapon[1];
    }

    if (app.keyboard[SDL_SCANCODE_3])
    {
        player->curr_weapon = player->list_weapon[2];
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
    char is_tank_shoot = 0, is_tank_hit = 0;

    for (int curr_bull_ind = 0; curr_bull_ind < player->curr_weapon.bullet_count; curr_bull_ind++)
    {
        if (player->bullet[curr_bull_ind].is_shoot)
        {
            is_tank_shoot = 1;
            update_bullet(player, curr_bull_ind);
        }

        else if (player->bullet[curr_bull_ind].is_hit)
        {
            is_tank_shoot = 1;
            update_hit(player, curr_bull_ind);
        }

        else
        {
            player->bullet[curr_bull_ind].position.x = player->muzzle.end_x;
            player->bullet[curr_bull_ind].position.y = player->muzzle.end_y;
        }
    }

    if (player->was_shoot == 1)
    {
        if (is_tank_shoot == 0 && is_tank_hit == 0)
        {
            player->was_shoot = 0;
            player->was_touch = 0;
            swap_player();
        }
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
    draw_player(other_player);
    draw_player(curr_player);
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

    for (int curr_bull_ind = 0; curr_bull_ind < player->curr_weapon.bullet_count; curr_bull_ind++)
    {
        if (player->bullet[curr_bull_ind].is_shoot)
        {
            draw_bullet(player, curr_bull_ind);
        }

        if (player->bullet[curr_bull_ind].is_hit)
        {
            draw_hit(player, curr_bull_ind);
        }

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
    char angle[100], power[100], points[100], damage[100], weapon[255];

    sprintf(angle, "Current angle: %d", player->muzzle.degrees % 360);
    sprintf(power, "Current power: %d", player->power);
    sprintf(points, "Current points: %d", player->points);
    sprintf(weapon, "Current weapon: %s", player->curr_weapon.weapon_name);
    sprintf(damage, "+%d", player->curr_weapon.damage);

    draw_text(angle, 50, 50, player->color.r, player->color.g, player->color.b);
    draw_text(power, 50, 80, player->color.r, player->color.g, player->color.b);
    draw_text(points, 50, 20, player->color.r, player->color.g, player->color.b);
    draw_text(weapon, 50, 110, player->color.r, player->color.g, player->color.b);

    if (player->was_touch)
    {
        draw_text(damage, other_player->size.x + 20, other_player->size.y - 20, player->color.r, player->color.g, player->color.b);
    }
}

static void update_bullet(Tank *player, int bullet_ind)
{
    collision col = check_earth_collision(player->bullet[bullet_ind]);
    if (col == EARTH_COLLISION || col == TANK_COLLISION)
    {
        player->bullet[bullet_ind].is_shoot = 0;
        player->bullet[bullet_ind].is_hit = 1;
        return;
    }

    double speed_up = 3;
    double t = speed_up * (SDL_GetTicks() - player->shoot_time) / 1000.0;
    double x = speed_up * player->power * t * cos(player->muzzle.angle);
    double y = speed_up * player->power * t * sin(player->muzzle.angle) + speed_up * 0.5 * G * t * t;

    player->bullet[bullet_ind].position.x = player->muzzle.end_x + x + bullet_ind * 10;
    player->bullet[bullet_ind].position.y = player->muzzle.end_y + y + bullet_ind * 7;
}

static void update_hit(Tank *player, int bullet_ind)
{
    if (player->bullet[bullet_ind].is_cur_up == 0)
    {
        player->bullet[bullet_ind].cur_rad++;
    }

    else
    {
        player->bullet[bullet_ind].cur_rad--;
    }

    if (player->bullet[bullet_ind].cur_rad == player->curr_weapon.max_radius)
    {
        if (check_tank_collision(other_player,
                                 player->bullet[bullet_ind].position.x + player->bullet[bullet_ind].position.w / 2,
                                 player->bullet[bullet_ind].position.y + player->bullet[bullet_ind].position.h / 2,
                                 player->bullet[bullet_ind].cur_rad) == TANK_COLLISION)
        {
            player->points += player->curr_weapon.damage;
            player->was_touch = 1;
            drop_player(other_player);
        }

        drop_earth(player->bullet[bullet_ind].position.x + player->bullet[bullet_ind].position.w / 2,
                   player->bullet[bullet_ind].position.y + player->bullet[bullet_ind].position.h / 2,
                   player->bullet[bullet_ind].cur_rad);

        player->bullet[bullet_ind].is_cur_up = 1;
    }

    if (player->bullet[bullet_ind].cur_rad == -1)
    {
        player->bullet[bullet_ind].is_hit = 0;
        player->bullet[bullet_ind].cur_rad = 0;
        player->bullet[bullet_ind].is_cur_up = 0;
    }
}

static void draw_hit(Tank *player, int bullet_ind)
{
    filledCircleRGBA(app.renderer,
                     player->bullet[bullet_ind].position.x + player->bullet[bullet_ind].position.w / 2,
                     player->bullet[bullet_ind].position.y + player->bullet[bullet_ind].position.h / 2,
                     player->bullet[bullet_ind].cur_rad,
                     255,
                     141 + player->bullet[bullet_ind].cur_rad,
                     27, 255);
}

static void draw_bullet(Tank *player, int bullet_ind)
{
    blit(player->bullet[bullet_ind].texture, &player->bullet[bullet_ind].position, 1);
}

static void drop_earth(int x, int y, int r)
{
    double d;
    for (int x_map = x - r; x_map <= x + r; x_map++)
    {
        for (int y_map = y - r; y_map <= y + r; y_map++)
        {

            if (x_map < 0 || y_map < 0)
            {
                continue;
            }

            if (x_map > SCREEN_WIDTH)
            {
                x_map = SCREEN_WIDTH - 1;
            }

            if (y_map > SCREEN_HEIGHT)
            {
                y_map = SCREEN_HEIGHT - 1;
            }

            d = SDL_sqrt((SDL_pow((double)(x - x_map), 2.0f) + SDL_pow((double)(y - y_map), 2.0f)));


            if (d <= (double)(r))
            {
                pixel_map[x_map][y_map] = 0;
            }

        }
    }

    set_map();
}

static collision check_tank_collision(Tank *player, int x, int y, int r)
{
    int w1[2] = {player->size.x, player->size.y};
    int w2[2] = {player->size.x + player->size.w, player->size.y};
    int w3[2] = {player->size.x, player->size.y + player->size.h};
    int w4[2] = {player->size.x + player->size.w, player->size.y + player->size.h};

    double d1 = SDL_sqrt((SDL_pow((double)(x - w1[0]), 2.0f) + SDL_pow((double)(y - w1[1]), 2.0f)));
    double d2 = SDL_sqrt((SDL_pow((double)(x - w2[0]), 2.0f) + SDL_pow((double)(y - w2[1]), 2.0f)));
    double d3 = SDL_sqrt((SDL_pow((double)(x - w3[0]), 2.0f) + SDL_pow((double)(y - w3[1]), 2.0f)));
    double d4 = SDL_sqrt((SDL_pow((double)(x - w4[0]), 2.0f) + SDL_pow((double)(y - w4[1]), 2.0f)));
    double d_r = (double)(r);

    if (d1 <= d_r || d2 <= d_r || d3 <= d_r || d4 <= d_r)
    {
        return TANK_COLLISION;
    }

    return NO_COLLISION;
}

static collision check_earth_collision(Bullet bullet)
{
    if (bullet.position.x < 0 || bullet.position.x > SCREEN_WIDTH)
    {
        return EARTH_COLLISION;
    }

    if (bullet.position.y >= 0 && bullet.position.y < SCREEN_HEIGHT &&
        pixel_map[(bullet.position.x) % SCREEN_WIDTH][(bullet.position.y + bullet.position.h) % SCREEN_HEIGHT] == 1)
    {
        return EARTH_COLLISION;
    }

    if (bullet.position.x >= other_player->size.x && 
        bullet.position.x <= (other_player->size.x + other_player->size.w) &&
        bullet.position.y >= other_player->size.y &&
        bullet.position.y <= (other_player->size.y + other_player->size.h))
    {
        return TANK_COLLISION;
    }

    return NO_COLLISION;
}

static void swap_player(void)
{
    if (curr_player == player1)
    {
        curr_player = player2;
        other_player = player1;
    }
    else
    {
        curr_player = player1;
        other_player = player2;
    }
}
