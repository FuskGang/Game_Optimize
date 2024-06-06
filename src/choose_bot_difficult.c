#include "choose_bot_difficult.h"

static void draw(void);
static void update(void);
static void easy_mode(void);
static void medium_mode(void);
static void hard_mode(void);
static void set_difficult(void);

static int x_first_button_text = 500;
static int y_first_button_text = 200;

static int difficult = 0;

void init_choose_difficult(void)
{
    clear_widgets();
    Widget *w;

    w = create_widget("easy");
    w->x = x_first_button_text;
    w->y = y_first_button_text + 0;
    strcpy(w->label, "Лёгкий уровень сложности");
    w->action = easy_mode;

    app.active_widget = w;

    w = create_widget("medium");
    w->x = x_first_button_text;
    w->y = y_first_button_text + 50;
    strcpy(w->label, "Средний уровень сложности");
    w->action = medium_mode;

    w = create_widget("hard");
    w->x = x_first_button_text;
    w->y = y_first_button_text + 100;
    strcpy(w->label, "Тяжёлый уровень сложности");
    w->action = hard_mode;

    app.delegate.draw = draw;
    app.delegate.update = update;
}

static void draw(void)
{
    draw_widgets();

    if (game_settings.is_first_player_bot)
    {
        if (!game_settings.first_player_difficult)
        {
            draw_text("Сложность первого компьютера:", x_first_button_text - 40, y_first_button_text - 50, 255, 255, 255);
        }
        else
        {
            draw_text("Сложность второго компьютера:", x_first_button_text - 40, y_first_button_text - 50, 255, 255, 255);
        }
    }
    else
    {
        draw_text("Сложность компьютера:", x_first_button_text - 40, y_first_button_text - 50, 255, 255, 255);
    }
}

static void update(void)
{
    do_widgets();
    if (app.keyboard[SDL_SCANCODE_ESCAPE])
    {
        app.keyboard[SDL_SCANCODE_ESCAPE] = 0;
        game_settings.is_first_player_bot = SDL_FALSE;
        game_settings.is_second_player_bot = SDL_FALSE;
        game_settings.first_player_difficult = 0;
        game_settings.second_player_difficult = 0;
        init_menu();
    }
}

static void easy_mode(void)
{
    difficult = EASY;
    set_difficult();
}

static void medium_mode(void)
{
    difficult = MEDIUM;
    set_difficult();
}

static void hard_mode(void)
{
    difficult = HARD;
    set_difficult();
}

static void set_difficult(void)
{
    if (game_settings.is_first_player_bot)
    {
        if (!game_settings.first_player_difficult)
        {
            game_settings.first_player_difficult = difficult;
        }
        else
        {
            game_settings.second_player_difficult = difficult;
            init_choose_name();
        }
    }
    else
    {
        game_settings.second_player_difficult = difficult;
        init_choose_name();
    }
}