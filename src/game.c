#include "game.h"

static void update(void);
static void draw(void);
static void init_player(Tank *player, ArsenalItem *arsenal, char *name, int x_coord, SDL_bool is_bot);
static void get_player(Tank *player, char *name,
                       int player_r, int player_g, int player_b,
                       int size_x, int size_y,
                       char *bullet_texture_path, int bullet_r, int bullet_g, int bullet_b,
                       char *player_texture_path);
static void drop_player(Tank *);
static void draw_player(Tank *);
static void draw_stats(Tank *);
static void do_human_input(Tank *);
static void do_bot_input(Tank *);
static void update_bullet(Tank *, int);
static void update_tank(Tank *);
static void update_hit(Tank *, int);
static void draw_hit(Tank *, int);
static void draw_bullet(Tank *, int);
static void draw_final_screen(void);
static void free_resources(void);
static void drop_earth(int, int, int);
static SDL_bool do_test_shoot(void);
static collision check_earth_collision(Bullet);
static collision check_tank_collision(Tank *, int, int, int);
static void swap_player(void);
static float calculate_delta_time(void);

static int curr_move;
static Tank *player1;
static Tank *player2;
static Tank *curr_player;
static Tank *other_player;
static float delta_time;

void init_game(ArsenalItem *left_arsenal, ArsenalItem *right_arsenal)
{
    init_map();

    player1 = malloc(sizeof(Tank));
    player2 = malloc(sizeof(Tank));

    init_player(player1, left_arsenal, "First player", 100, SDL_TRUE);
    init_player(player2, right_arsenal, "Second player", SCREEN_WIDTH - 400, SDL_TRUE);

    curr_player = player1;
    other_player = player2;

    app.delegate.update = update;
    app.delegate.draw = draw;
}

static void init_player(Tank *player, ArsenalItem *arsenal, char *name, int x_coord, SDL_bool is_bot)
{
    player->arsenal = arsenal;

    get_player(player, name,
               rand() % 156 + 100, rand() % 156 + 100, rand() % 156 + 100,
               x_coord, 0,
               "assets/Bullet2.png", 255, 0, 0,
               "assets/Tank.png");

    player->is_bot = is_bot;

    srand(time(NULL));

    for (int i = 0; i < arsenal[0].count; i++)
    {
        player->weapon_order[i] = 0;
    }

    for (int i = arsenal[0].count; i < arsenal[0].count + arsenal[1].count; i++)
    {
        player->weapon_order[i] = 1;
    }

    for (int i = arsenal[0].count + arsenal[1].count; i < TOTAL_WEAPONS / 2; i++)
    {
        player->weapon_order[i] = 2;
    }

    shuffle(player->weapon_order, TOTAL_WEAPONS / 2);

    curr_move = 0;

    drop_player(player);
}

static void get_player(Tank *player, char *name,
                       int player_r, int player_g, int player_b,
                       int size_x, int size_y,
                       char *bullet_texture_path, int bullet_r, int bullet_g, int bullet_b,
                       char *player_texture_path)
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
    player->size.w = TANK_SIZE_W;
    player->size.h = TANK_SIZE_H;

    player->muzzle.length = MUZZLE_LENGTH;
    player->muzzle.thickness = MUZZLE_THICKNESS;
    player->muzzle.degrees = MUZZLE_DEGREES;

    for (int i = 0; i < MAX_BULLET; i++)
    {
        player->bullets[i].texture = bullet_texture;
        player->bullets[i].position.w = BULLET_W;
        player->bullets[i].position.h = BULLET_H;
        player->bullets[i].is_radius_up = 0;
        player->bullets[i].curr_radius = 0;
        player->bullets[i].is_hit = 0;
        player->bullets[i].is_shoot = 0;
    }

    player->power = POWER;
    player->texture = player_texture;
    player->input_time = 0;
    player->last_shoot_time = 0;
    player->points = 0;
    player->damage_target = DAMAGE_TARGET_NONE;
    player->is_shoot = 0;

    player->curr_weapon = player->arsenal[0].weapon;
}

static void drop_player(Tank *player)
{
    player->size.y = 0;
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

    SDL_Point center = {player->size.x + player->size.w / 2, player->size.y + player->size.h};

    rotate_point(player->size.x, player->size.y, center.x, center.y, player->degrees, &player->bounding_box[0].x, &player->bounding_box[0].y);
    rotate_point(player->size.x + player->size.w, player->size.y, center.x, center.y, player->degrees, &player->bounding_box[1].x, &player->bounding_box[1].y);
    rotate_point(player->size.x + player->size.w, player->size.y + player->size.h, center.x, center.y, player->degrees, &player->bounding_box[2].x, &player->bounding_box[2].y);
    rotate_point(player->size.x, player->size.y + player->size.h, center.x, center.y, player->degrees, &player->bounding_box[3].x, &player->bounding_box[3].y);
}

static void do_human_input(Tank *player)
{
    if (app.keyboard[SDL_SCANCODE_ESCAPE])
    {
        app.keyboard[SDL_SCANCODE_ESCAPE] = 0;
        free_resources();
        init_menu();
    }

    if (player->is_shoot)
    {
        return;
    }

    if ((SDL_GetTicks() - player->input_time < DELAY_HUMAN_INPUT))
    {
        return;
    }

    if (app.keyboard[SDL_SCANCODE_SPACE])
    {
        player->is_shoot = 1;
        return;
    }

    if (app.keyboard[SDL_SCANCODE_1])
    {
        player->input_time = SDL_GetTicks();

        player->curr_weapon = player->arsenal[0].weapon;
    }

    if (app.keyboard[SDL_SCANCODE_2])
    {
        player->input_time = SDL_GetTicks();

        player->curr_weapon = player->arsenal[1].weapon;
    }

    if (app.keyboard[SDL_SCANCODE_3])
    {
        player->input_time = SDL_GetTicks();

        player->curr_weapon = player->arsenal[2].weapon;
    }

    if (app.keyboard[SDL_SCANCODE_RIGHT])
    {
        player->input_time = SDL_GetTicks();

        player->muzzle.degrees += 1;

        if (player->muzzle.degrees > 359)
        {
            player->muzzle.degrees -= 360;
        }
    }

    if (app.keyboard[SDL_SCANCODE_LEFT])
    {
        player->input_time = SDL_GetTicks();

        player->muzzle.degrees -= 1;

        if (player->muzzle.degrees < 0)
        {
            player->muzzle.degrees += 360;
        }
    }

    if (app.keyboard[SDL_SCANCODE_UP])
    {
        player->input_time = SDL_GetTicks();

        if (player->power < 100)
        {
            player->power += 1;
        }
    }

    if (app.keyboard[SDL_SCANCODE_DOWN])
    {
        player->input_time = SDL_GetTicks();

        if (player->power > 1)
        {
            player->power -= 1;
        }
    }

    if (app.keyboard[SDL_SCANCODE_Z])
    {
        player->input_time = SDL_GetTicks();

        if (player->size.x > 1)
        {
            player->size.x -= 1;
        }
    }

    if (app.keyboard[SDL_SCANCODE_X])
    {
        player->input_time = SDL_GetTicks();

        if (player->size.x + player->size.w < SCREEN_WIDTH)
        {
            player->size.x += 1;
        }
    }

    // if (app.keyboard[SDL_SCANCODE_B])
    // {
    //     if (do_test_shoot() == SDL_FALSE)
    //     {
    //         player->muzzle.degrees += 1;

    //         if (player->muzzle.degrees < 0)
    //         {
    //             player->muzzle.degrees += 360;
    //         }
    //     }
    // }
}

static void do_bot_input(Tank *bot)
{
    SDL_bool is_success_shoot;

    if (app.keyboard[SDL_SCANCODE_ESCAPE])
    {
        app.keyboard[SDL_SCANCODE_ESCAPE] = 0;
        free_resources();
        init_menu();
    }

    if (bot->is_shoot)
    {
        return;
    }

    if ((SDL_GetTicks() - bot->input_time < DELAY_BOT_INPUT))
    {
        return;
    }

    is_success_shoot = do_test_shoot();

    if (bot->power < 60)
    {
        bot->input_time = SDL_GetTicks();

        if (bot->power < 100)
        {
            bot->power += 1;
        }

        return;
    }

    if (bot->power > 80)
    {
        bot->input_time = SDL_GetTicks();

        if (bot->power > 1)
        {
            bot->power -= 1;
        }

        return;
    }

    if (!is_success_shoot)
    {
        if (other_player->size.x > curr_player->size.x)
        {
            bot->input_time = SDL_GetTicks();

            bot->muzzle.degrees += 1;

            if (bot->muzzle.degrees > 359)
            {
                bot->muzzle.degrees -= 360;
            }
        }

        else
        {
            bot->input_time = SDL_GetTicks();

            bot->muzzle.degrees -= 1;

            if (bot->muzzle.degrees < 0)
            {
                bot->muzzle.degrees += 360;
            }
        }

        return;
    }

    else
    {
        bot->is_shoot = 1;
        return;
    }
}

static void update(void)
{
    delta_time = calculate_delta_time();

    if (curr_move == TOTAL_MOVES)
    {
        return;
    }

    if (curr_player->is_bot)
    {
        do_bot_input(curr_player);
    }

    else
    {
        do_human_input(curr_player);
    }

    update_tank(player1);
    update_tank(player2);
}

static void update_tank(Tank *player)
{
    player->muzzle.start_x = player->size.x + player->size.w / 2;
    player->muzzle.start_y = player->size.y + player->size.h / 4;
    player->muzzle.angle = (player->muzzle.degrees - 90) * M_PI / 180;
    player->muzzle.end_x = player->muzzle.start_x + player->muzzle.length * cos(player->muzzle.angle);
    player->muzzle.end_y = player->muzzle.start_y + player->muzzle.length * sin(player->muzzle.angle);

    if (player->bounding_box[0].x != player->size.x)
    {
        drop_player(player);
    }

    char is_tank_shoot = 0;

    if (!player->is_shoot)
        return;

    for (int curr_bull_ind = 0; curr_bull_ind < player->curr_weapon.max_bullet_count; curr_bull_ind++)
    {
        if (player->curr_weapon.current_bullet_count != player->curr_weapon.max_bullet_count &&
            !player->bullets[curr_bull_ind].is_shoot && !player->bullets[curr_bull_ind].is_hit)
        {
            if ((SDL_GetTicks() - player->last_shoot_time) < DELAY_BETWEEN_BULLET && player->last_shoot_time != 0)
            {
                continue;
            }

            player->curr_weapon.current_bullet_count++;
            player->bullets[curr_bull_ind].is_shoot = 1;
            player->bullets[curr_bull_ind].angle = player->muzzle.angle;
            player->bullets[curr_bull_ind].shoot_time = SDL_GetTicks();
            player->bullets[curr_bull_ind].position.x = player->muzzle.end_x;
            player->bullets[curr_bull_ind].position.y = player->muzzle.end_y;
            player->last_shoot_time = SDL_GetTicks();
        }

        if (player->bullets[curr_bull_ind].is_shoot)
        {
            is_tank_shoot = 1;
            update_bullet(player, curr_bull_ind);
        }
        else if (player->bullets[curr_bull_ind].is_hit)
        {
            is_tank_shoot = 1;
            update_hit(player, curr_bull_ind);
        }
    }

    if (player->curr_weapon.hit_bullet_count == player->curr_weapon.max_bullet_count)
    {
        if (is_tank_shoot == 0)
        {
            if (SDL_GetTicks() - player->touch_time > DELAY_POINTS)
            {
                player->is_shoot = 0;
                player->damage_target = DAMAGE_TARGET_NONE;
                player->curr_weapon.current_bullet_count = 0;
                player->curr_weapon.hit_bullet_count = 0;
                swap_player();
            }
        }
    }
}

static void update_bullet(Tank *player, int bullet_ind)
{
    collision col = check_earth_collision(player->bullets[bullet_ind]);

    if (col == COLLISION_EARTH || col == COLLISION_TANK)
    {
        player->bullets[bullet_ind].is_shoot = 0;
        player->bullets[bullet_ind].is_hit = 1;
        player->bullets[bullet_ind].radius_time = SDL_GetTicks();
        return;
    }

    double t = SPEED_UP * (SDL_GetTicks() - player->bullets[bullet_ind].shoot_time) / 1000.0;
    double x = SPEED_UP * player->power * t * cos(player->muzzle.angle + bullet_ind * 0.01);
    double y = SPEED_UP * player->power * t * sin(player->muzzle.angle + bullet_ind * 0.01) + SPEED_UP * 0.5 * G * t * t;

    player->bullets[bullet_ind].position.x = player->muzzle.end_x + x;
    player->bullets[bullet_ind].position.y = player->muzzle.end_y + y;
}

static void update_hit(Tank *player, int bullet_ind)
{
    if (SDL_GetTicks() - player->bullets[bullet_ind].radius_time < DELAY_RADIUS)
    {
        return;
    }

    player->bullets[bullet_ind].radius_time = SDL_GetTicks();

    if (player->bullets[bullet_ind].is_radius_up == 0)
    {
        player->bullets[bullet_ind].curr_radius++;
    }

    else
    {
        player->bullets[bullet_ind].curr_radius--;
    }

    if (player->bullets[bullet_ind].curr_radius == player->curr_weapon.max_radius)
    {
        if (check_tank_collision(other_player,
                                 player->bullets[bullet_ind].position.x + player->bullets[bullet_ind].position.w / 2,
                                 player->bullets[bullet_ind].position.y + player->bullets[bullet_ind].position.h / 2,
                                 player->bullets[bullet_ind].curr_radius) == COLLISION_TANK)
        {
            player->points += player->curr_weapon.damage;
            player->damage_target = DAMAGE_TARGET_OTHER;
            player->touch_time = SDL_GetTicks();
            drop_player(other_player);
        }

        if (check_tank_collision(curr_player,
                                 player->bullets[bullet_ind].position.x + player->bullets[bullet_ind].position.w / 2,
                                 player->bullets[bullet_ind].position.y + player->bullets[bullet_ind].position.h / 2,
                                 player->bullets[bullet_ind].curr_radius) == COLLISION_TANK)
        {
            other_player->points += player->curr_weapon.damage;

            if (player->damage_target == DAMAGE_TARGET_OTHER)
            {
                player->damage_target = DAMAGE_TARGET_BOTH;
            }

            else
            {
                player->damage_target = DAMAGE_TARGET_CURRENT;
            }

            player->touch_time = SDL_GetTicks();
            drop_player(curr_player);
        }

        drop_earth(player->bullets[bullet_ind].position.x + player->bullets[bullet_ind].position.w / 2,
                   player->bullets[bullet_ind].position.y + player->bullets[bullet_ind].position.h / 2,
                   player->bullets[bullet_ind].curr_radius);

        player->bullets[bullet_ind].is_radius_up = 1;
    }

    if (player->bullets[bullet_ind].curr_radius == -1)
    {
        player->curr_weapon.hit_bullet_count++;
        player->bullets[bullet_ind].is_hit = 0;
        player->bullets[bullet_ind].curr_radius = 0;
        player->bullets[bullet_ind].is_radius_up = 0;
    }
}

static void draw(void)
{
    draw_pixel_map();
    draw_player(other_player);
    draw_player(curr_player);

    if (curr_move == TOTAL_MOVES)
    {
        draw_final_screen();
        return;
    }

    draw_stats(curr_player);
}

static void draw_player(Tank *player)
{
    if (player->size.y + player->size.h > SCREEN_HEIGHT)
    {
        player->size.y = SCREEN_HEIGHT - player->size.h;
    }

    SDL_RenderDrawLine(app.renderer, player->bounding_box[0].x, player->bounding_box[0].y, player->bounding_box[1].x, player->bounding_box[1].y);
    SDL_RenderDrawLine(app.renderer, player->bounding_box[1].x, player->bounding_box[1].y, player->bounding_box[2].x, player->bounding_box[2].y);
    SDL_RenderDrawLine(app.renderer, player->bounding_box[2].x, player->bounding_box[2].y, player->bounding_box[3].x, player->bounding_box[3].y);
    SDL_RenderDrawLine(app.renderer, player->bounding_box[3].x, player->bounding_box[3].y, player->bounding_box[0].x, player->bounding_box[0].y);

    blit_rotated(player->texture, &player->size, FALSE, player->degrees);
    thickLineRGBA(app.renderer,
                  player->muzzle.start_x,
                  player->muzzle.start_y,
                  player->muzzle.end_x,
                  player->muzzle.end_y,
                  player->muzzle.thickness,
                  150, 150, 150, 255);

    for (int curr_bull_ind = 0; curr_bull_ind < player->curr_weapon.max_bullet_count; curr_bull_ind++)
    {
        if (player->bullets[curr_bull_ind].is_shoot)
        {
            draw_bullet(player, curr_bull_ind);
        }

        if (player->bullets[curr_bull_ind].is_hit)
        {
            draw_hit(player, curr_bull_ind);
        }
    }
}

static void draw_stats(Tank *player)
{
    char angle[100], power[100], points[100], damage[100], weapon[255], move[100];

    sprintf(angle, "Угол: %d", player->muzzle.degrees % 360);
    sprintf(power, "Мощность: %d", player->power);
    sprintf(points, "Очки: %d", player->points);
    sprintf(move, "Текущий ход: %d", curr_move + 1);
    sprintf(weapon, "Оружие: %s", player->curr_weapon.weapon_name);
    sprintf(damage, "+%d", player->curr_weapon.damage);

    draw_text(points, 50, 20, player->color.r, player->color.g, player->color.b);
    draw_text(angle, 50, 50, player->color.r, player->color.g, player->color.b);
    draw_text(power, 50, 80, player->color.r, player->color.g, player->color.b);
    draw_text(move, SCREEN_WIDTH / 2 - 60, 5, 255, 255, 255);
    draw_text(weapon, 50, 110, player->color.r, player->color.g, player->color.b);

    if (player->damage_target == DAMAGE_TARGET_OTHER || player->damage_target == DAMAGE_TARGET_BOTH)
    {
        draw_text(damage, player->size.x + 20, player->size.y - 20 - (SDL_GetTicks() - player->touch_time) / 5, player->color.r, player->color.g, player->color.b);
    }

    if (player->damage_target == DAMAGE_TARGET_CURRENT || player->damage_target == DAMAGE_TARGET_BOTH)
    {
        draw_text(damage, other_player->size.x + 20, other_player->size.y - 20 - (SDL_GetTicks() - player->touch_time) / 5, other_player->color.r, other_player->color.g, other_player->color.b);
    }
}

static void draw_hit(Tank *player, int bullet_ind)
{
    filledCircleRGBA(app.renderer,
                     player->bullets[bullet_ind].position.x + player->bullets[bullet_ind].position.w / 2,
                     player->bullets[bullet_ind].position.y + player->bullets[bullet_ind].position.h / 2,
                     player->bullets[bullet_ind].curr_radius,
                     255,
                     141 + player->bullets[bullet_ind].curr_radius,
                     27, 255);
}

static void draw_bullet(Tank *player, int bullet_ind)
{
    blit(player->bullets[bullet_ind].texture, &player->bullets[bullet_ind].position, 1);
}

static void draw_final_screen(void)
{
    char game_over[355] = "", winner_text[355] = "", loser_text[355] = "", press_key[] = "Press esc to exit game";

    sprintf(game_over, "Game over!");

    if (curr_player->points > other_player->points)
    {
        sprintf(winner_text, "Winner - %s with %d points", curr_player->player_name, curr_player->points);
        sprintf(loser_text, "Loser - %s with %d points", other_player->player_name, other_player->points);
    }

    else if (curr_player->points < other_player->points)
    {
        sprintf(winner_text, "Winner - %s with %d points", other_player->player_name, other_player->points);
        sprintf(loser_text, "Loser - %s with %d points", curr_player->player_name, curr_player->points);
    }

    else
    {
        sprintf(winner_text, "Draw with %d", curr_player->points);
        sprintf(loser_text, " ");
    }

    draw_text(game_over, SCREEN_WIDTH / 2 - 50, 100, 255, 255, 255);
    draw_text(winner_text, SCREEN_WIDTH / 2 - 50, 120, 255, 255, 255);
    draw_text(loser_text, SCREEN_WIDTH / 2 - 50, 140, 255, 255, 255);
    draw_text(press_key, SCREEN_WIDTH / 2 - 50, 160, 255, 255, 255);
}

static void free_resources(void)
{
    free(player1->arsenal);
    free(player2->arsenal);
    free(player1);
    free(player2);
}

static void drop_earth(int x, int y, int r)
{
    double d;

    for (int x_map = (x - r); x_map <= (x + r); x_map++)
    {
        for (int y_map = (y - r); y_map <= (y + r); y_map++)
        {
            if (x_map < 0 || y_map < 0)
            {
                continue;
            }

            if (x_map > SCREEN_WIDTH)
            {
                continue;
            }

            if (y_map > SCREEN_HEIGHT)
            {
                continue;
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

static SDL_bool do_test_shoot(void)
{
    int bullet_poz_x = 0, bullet_poz_y = 0;
    Bullet test_bullet;

    memset(&test_bullet, 0, sizeof(test_bullet));

    test_bullet.position.x = 0;
    test_bullet.position.y = 0;
    test_bullet.position.h = 10;
    test_bullet.position.w = 10;

    for (double t = 0.0f; t < 30.0f; t = t + 0.00001)
    {
        if (check_earth_collision(test_bullet) == COLLISION_EARTH || check_earth_collision(test_bullet) == COLLISION_TANK)
        {

            if (check_tank_collision(other_player, bullet_poz_x + BULLET_W / 2, bullet_poz_y + BULLET_H / 2, curr_player->curr_weapon.max_radius) == COLLISION_TANK)
            {
                return SDL_TRUE;
            }

            return SDL_FALSE;
        }

        double x_offset = SPEED_UP * curr_player->power * t * cos(curr_player->muzzle.angle);
        double y_offset = SPEED_UP * curr_player->power * t * sin(curr_player->muzzle.angle) + SPEED_UP * 0.5 * G * t * t;

        bullet_poz_x = curr_player->muzzle.end_x + x_offset;
        bullet_poz_y = curr_player->muzzle.end_y + y_offset;

        test_bullet.position.x = bullet_poz_x;
        test_bullet.position.y = bullet_poz_y;
    }

    return SDL_FALSE;
}

static collision check_tank_collision(Tank *player, int x, int y, int r)
{
    const int w1[2] = {player->size.x, player->size.y};
    const int w2[2] = {player->size.x + player->size.w, player->size.y};
    const int w3[2] = {player->size.x, player->size.y + player->size.h};
    const int w4[2] = {player->size.x + player->size.w, player->size.y + player->size.h};

    double d1 = SDL_sqrt((SDL_pow((double)(x - w1[0]), 2.0f) + SDL_pow((double)(y - w1[1]), 2.0f)));
    double d2 = SDL_sqrt((SDL_pow((double)(x - w2[0]), 2.0f) + SDL_pow((double)(y - w2[1]), 2.0f)));
    double d3 = SDL_sqrt((SDL_pow((double)(x - w3[0]), 2.0f) + SDL_pow((double)(y - w3[1]), 2.0f)));
    double d4 = SDL_sqrt((SDL_pow((double)(x - w4[0]), 2.0f) + SDL_pow((double)(y - w4[1]), 2.0f)));
    double d_r = (double)(r);

    if (d1 < d_r || d2 < d_r || d3 < d_r || d4 < d_r)
    {
        return COLLISION_TANK;
    }

    if (x > w1[0] && x < w4[0] && y > w1[1] && y < w4[1])
    {
        return COLLISION_TANK;
    }

    if (((x + r) > w1[0] && (x - r) < w1[0] && (y + r) < w4[1] && (y - r) > w1[1]) ||
        ((y + r) > w1[1] && (y - r) < w1[1] && (x + r) < w4[0] && (x - r) > w1[0]) ||
        ((x - r) < w4[0] && (x + r) > w4[0] && (y + r) < w4[1] && (y - r) > w1[1]) ||
        ((y - r) < w4[1] && (y + r) > w4[1] && (x + r) < w4[0] && (x - r) > w1[0]))
    {
        return COLLISION_TANK;
    }

    return COLLISION_NONE;
}

static collision check_earth_collision(Bullet bullet)
{
    if (bullet.position.y > SCREEN_HEIGHT)
    {
        return COLLISION_EARTH;
    }

    if (bullet.position.x < 0 || bullet.position.x > SCREEN_WIDTH)
    {
        return COLLISION_EARTH;
    }

    if (bullet.position.y >= 0 && bullet.position.y < SCREEN_HEIGHT &&
        pixel_map[(bullet.position.x) % SCREEN_WIDTH][(bullet.position.y + bullet.position.h) % SCREEN_HEIGHT] == 1)
    {
        return COLLISION_EARTH;
    }

    if (bullet.position.x >= other_player->size.x &&
        bullet.position.x <= (other_player->size.x + other_player->size.w) &&
        bullet.position.y >= other_player->size.y &&
        bullet.position.y <= (other_player->size.y + other_player->size.h))
    {
        return COLLISION_TANK;
    }

    return COLLISION_NONE;
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
        curr_move++;
    }

    if (curr_player->is_bot)
    {
        curr_player->curr_weapon = curr_player->arsenal[curr_player->weapon_order[curr_move / 2]].weapon;
        curr_player->power = (rand() % 60 + 20);
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