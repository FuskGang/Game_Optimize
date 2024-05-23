#include "shop.h"

static void draw(void);
static void draw_line(void);
static void draw_shop(void);
static void draw_left_weapons(void);
static void draw_right_weapons(void);
static void update(void);
static void do_weapon_choice(void);
static Weapon *get_weapon(char *weapon_name, int damage, int max_radius, int max_bullet_count, SDL_Texture *texture);

static SDL_Texture *default_texture;
static SDL_Texture *deagle_texture;
static SDL_Texture *minigun_texture;
static Weapon weapons[TOTAL_WEAPONS], left_weapons[TOTAL_WEAPONS / 2], right_weapons[TOTAL_WEAPONS / 2];
static int remaining_weapons = TOTAL_WEAPONS, is_space_pressed = 0, index_order[TOTAL_WEAPONS];
static Uint32 last_time;

void init_shop(void)
{
    app.delegate.draw = draw;
    app.delegate.update = update;

    default_texture = load_texture("assets/default.png");
    deagle_texture = load_texture("assets/deagle.png");
    minigun_texture = load_texture("assets/minigun.png");

    Weapon *weapon_storage[3];

    weapon_storage[0] = get_weapon("Default", 10, 40, 1, default_texture);
    weapon_storage[1] = get_weapon("Deagle", 35, 25, 1, deagle_texture);
    weapon_storage[2] = get_weapon("Minigun", 5, 12, 5, minigun_texture);

    srand(time(NULL));

    for (int i = 0; i < TOTAL_WEAPONS; i++)
    {
        weapons[i] = *weapon_storage[rand() % 3];
        index_order[i] = i;
    }

    shuffle(index_order, TOTAL_WEAPONS);

    last_time = SDL_GetTicks();
}

void draw(void)
{
    draw_line();
    draw_shop();
    draw_left_weapons();
    draw_right_weapons();
}

static void draw_line(void)
{
    if (!is_space_pressed)
    {
        draw_text("Нажмите пробел, чтобы начать распределение оружия",
                  SCREEN_WIDTH / 2 - 250,
                  SCREEN_HEIGHT - 80,
                  255, 255, 255);
    }
}

static void draw_shop(void)
{
    SDL_Rect dest;
    dest.w = 140;
    dest.x = SCREEN_WIDTH / 2 - dest.w / 2 - 10;
    dest.h = dest.w / 4;

    for (int i = 0; i < TOTAL_WEAPONS / 2; i++)
    {
        if (weapons[i].texture != NULL)
        {
            dest.y = (dest.h + 10) * i + 100;
            blit(weapons[i].texture, &dest, 1);
        }
    }

    dest.x = SCREEN_WIDTH / 2 + dest.w / 2 + 10;
    for (int i = TOTAL_WEAPONS / 2; i < TOTAL_WEAPONS; i++)
    {
        if (weapons[i].texture != NULL)
        {
            dest.y = (dest.h + 10) * (i - TOTAL_WEAPONS / 2) + 100;
            blit(weapons[i].texture, &dest, 1);
        }
    }
}

static void draw_left_weapons(void)
{
    SDL_Rect dest;
    dest.w = 140;
    dest.x = 100;
    dest.y = 100;
    dest.h = dest.w / 4;

    for (int i = 0; i < TOTAL_WEAPONS / 2; i++)
    {
        if (left_weapons[i].texture != NULL)
        {
            blit(left_weapons[i].texture, &dest, 1);
            dest.y += dest.h + 20;
        }
    }
}

static void draw_right_weapons(void)
{
    SDL_Rect dest;
    dest.w = 140;
    dest.x = SCREEN_WIDTH - dest.w - 100;
    dest.y = 100;
    dest.h = dest.w / 4;

    for (int i = 0; i < TOTAL_WEAPONS / 2; i++)
    {
        if (right_weapons[i].texture != NULL)
        {
            blit(right_weapons[i].texture, &dest, 1);
            dest.y += dest.h + 20;
        }
    }
}

void update(void)
{
    if (app.keyboard[SDL_SCANCODE_ESCAPE])
    {
        app.keyboard[SDL_SCANCODE_ESCAPE] = 0;
        init_menu();
    }

    if (!is_space_pressed && app.keyboard[SDL_SCANCODE_SPACE])
    {
        is_space_pressed = 1;
    }

    if (is_space_pressed && remaining_weapons)
    {
        do_weapon_choice();
    }

    if (remaining_weapons == 0 && SDL_GetTicks() - last_time > 2000)
    {
        init_game(left_weapons, right_weapons);
    }
}

static void do_weapon_choice(void)
{
    if (SDL_GetTicks() - last_time > 200)
    {
        int index = index_order[TOTAL_WEAPONS - remaining_weapons];
        if (remaining_weapons % 2 == 0)
        {
            left_weapons[(TOTAL_WEAPONS - remaining_weapons + 1) / 2] = weapons[index];
        }
        else
        {
            right_weapons[(TOTAL_WEAPONS - remaining_weapons) / 2] = weapons[index];
        }
        weapons[index].texture = NULL;
        remaining_weapons--;
        last_time = SDL_GetTicks();
    }
}

static Weapon *get_weapon(char *weapon_name, int damage, int max_radius, int max_bullet_count, SDL_Texture *texture)
{
    Weapon *weapon;
    weapon = (Weapon *)malloc(sizeof(Weapon));
    strcpy(weapon->weapon_name, weapon_name);
    weapon->damage = damage;
    weapon->max_radius = max_radius;
    weapon->max_bullet_count = max_bullet_count;
    weapon->current_bullet_count = 0;
    weapon->hit_bullet_count = 0;
    weapon->texture = texture;

    return weapon;
}

