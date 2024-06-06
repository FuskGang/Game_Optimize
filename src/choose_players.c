#include "choose_players.h"

static void draw(void);
static void update(void);
static void pvp(void);
static void pve(void);
static void eve(void);

static int x_first_button_text = 500;
static int y_first_button_text = 200;

void init_choose_players(void)
{
    clear_widgets();
    Widget *w;

    w = create_widget("pvp");
    w->x = x_first_button_text;
    w->y = y_first_button_text + 0;
    strcpy(w->label, "Человек против человека");
    w->action = pvp;

    app.active_widget = w;

    w = create_widget("pve");
    w->x = x_first_button_text;
    w->y = y_first_button_text + 50;
    strcpy(w->label, "Человек против компьютера");
    w->action = pve;

    w = create_widget("eve");
    w->x = x_first_button_text;
    w->y = y_first_button_text + 100;
    strcpy(w->label, "Компьютер против компьютера");
    w->action = eve;

    app.delegate.draw = draw;
    app.delegate.update = update;
}

static void draw(void)
{
    draw_widgets();
    draw_text("Выберите режим игры: ", x_first_button_text - 40, y_first_button_text - 50, 255, 255, 255);
}

static void update(void)
{
    do_widgets();
    if (app.keyboard[SDL_SCANCODE_ESCAPE])
    {
        app.keyboard[SDL_SCANCODE_ESCAPE] = 0;
        game_settings.is_first_player_bot = SDL_FALSE;
        game_settings.is_second_player_bot = SDL_FALSE;
        init_menu();
    }
}

static void pvp(void)
{
    game_settings.is_first_player_bot = SDL_FALSE;
    game_settings.is_second_player_bot = SDL_FALSE;
    init_choose_name();
}

static void pve(void)
{
    game_settings.is_first_player_bot = SDL_FALSE;
    game_settings.is_second_player_bot = SDL_TRUE;
    init_choose_difficult();
}

static void eve(void)
{
    game_settings.is_first_player_bot = SDL_TRUE;
    game_settings.is_second_player_bot = SDL_TRUE;
    init_choose_difficult();
}