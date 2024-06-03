#include "choose_players.h"

static void draw(void);
static void update(void);

int x_first_button_text = 500;
int y_first_button_text = 200;

char player_player_button_text[] = "Человек против человека";
char player_bot_button_text[] = "Человек против компьютера";
char bot_bot_button_text[] = "Компьютер против компьютера";
char *curr_choose_text;

void init_choose_players(void)
{
    app.delegate.draw = draw;
    app.delegate.update = update;
    curr_choose_text = player_player_button_text;
}

static void draw(void)
{
    if (curr_choose_text == player_player_button_text)
    {
        draw_text(">", x_first_button_text - 40, y_first_button_text, 0, 255, 0);
        draw_text(player_player_button_text, x_first_button_text, y_first_button_text, 0, 255, 0);
        draw_text(player_bot_button_text, x_first_button_text, y_first_button_text + 50, 255, 255, 255);
        draw_text(bot_bot_button_text, x_first_button_text, y_first_button_text + 100, 255, 255, 255);
    }

    if (curr_choose_text == player_bot_button_text)
    {
        draw_text(player_player_button_text, x_first_button_text, y_first_button_text, 255, 255, 255);
        draw_text(">", x_first_button_text - 40, y_first_button_text + 50, 0, 255, 0);
        draw_text(player_bot_button_text, x_first_button_text, y_first_button_text + 50, 0, 255, 0);
        draw_text(bot_bot_button_text, x_first_button_text, y_first_button_text + 100, 255, 255, 255);
    }

    if (curr_choose_text == bot_bot_button_text)
    {
        draw_text(player_player_button_text, x_first_button_text, y_first_button_text, 255, 255, 255);
        draw_text(player_bot_button_text, x_first_button_text, y_first_button_text + 50, 255, 255, 255);
        draw_text(">", x_first_button_text - 40, y_first_button_text + 100, 0, 255, 0);
        draw_text(bot_bot_button_text, x_first_button_text, y_first_button_text + 100, 0, 255, 0);
    }
}

static void update(void)
{
    if (app.keyboard[SDL_SCANCODE_ESCAPE])
    {
        app.keyboard[SDL_SCANCODE_ESCAPE] = 0;
        init_menu();
    }

    if (app.keyboard[SDL_SCANCODE_RETURN])
    {
        app.keyboard[SDL_SCANCODE_RETURN] = 0;

        if (curr_choose_text == player_player_button_text)
        {
            game_settings.is_first_player_bot = SDL_FALSE;
            game_settings.is_second_player_bot = SDL_FALSE;
        }

        if (curr_choose_text == player_bot_button_text)
        {
            game_settings.is_first_player_bot = SDL_FALSE;
            game_settings.is_second_player_bot = SDL_TRUE;
        }

        if (curr_choose_text == bot_bot_button_text)
        {
            game_settings.is_first_player_bot = SDL_TRUE;
            game_settings.is_second_player_bot = SDL_TRUE;
        }

        init_shop();
    }

    if (app.keyboard[SDL_SCANCODE_UP])
    {
        app.keyboard[SDL_SCANCODE_UP] = 0;

        if (curr_choose_text == player_player_button_text)
        {
            curr_choose_text = bot_bot_button_text;
        }

        else if (curr_choose_text == player_bot_button_text)
        {
            curr_choose_text = player_player_button_text;
        }

        else if (curr_choose_text == bot_bot_button_text)
        {
            curr_choose_text = player_bot_button_text;
        }
    }

    if (app.keyboard[SDL_SCANCODE_DOWN])
    {
        app.keyboard[SDL_SCANCODE_DOWN] = 0;

        if (curr_choose_text == player_player_button_text)
        {
            curr_choose_text = player_bot_button_text;
        }

        else if (curr_choose_text == player_bot_button_text)
        {
            curr_choose_text = bot_bot_button_text;
        }

        else if (curr_choose_text == bot_bot_button_text)
        {
            curr_choose_text = player_player_button_text;
        }
    }
}
