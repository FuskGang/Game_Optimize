#include "choose_bot_difficult.h"

static void draw(void);
static void update(void);

static int x_first_button_text = 500;
static int y_first_button_text = 200;

static char easy_button_text[] = "Легкий уровень сложности";
static char medium_button_text[] = "Средний уровень сложности";
static char hard_button_text[] = "Тяжелый уровень сложности";
static char *curr_choose_text;

void init_choose_difficult(void)
{
    app.delegate.draw = draw;
    app.delegate.update = update;
    curr_choose_text = easy_button_text;
}

static void draw(void)
{
    if (game_settings.is_first_player_bot)
    {
        
        if (!game_settings.first_player_difficult)
        {
            draw_text("Выберите уровень сложности первому компьютеру", x_first_button_text, y_first_button_text - 50, 255, 255, 255);
        }
        else
        {
            draw_text("Выберите уровень сложности второму компьютеру", x_first_button_text, y_first_button_text - 50, 255, 255, 255);
        }
    }

    else
    {
        draw_text("Выберите уровень сложности компьютеру", x_first_button_text, y_first_button_text - 50, 255, 255, 255);
    }

    if (curr_choose_text == easy_button_text)
    {
        draw_text(">", x_first_button_text - 40, y_first_button_text, 0, 255, 0);
        draw_text(easy_button_text, x_first_button_text, y_first_button_text, 0, 255, 0);
        draw_text(medium_button_text, x_first_button_text, y_first_button_text + 50, 255, 255, 255);
        draw_text(hard_button_text, x_first_button_text, y_first_button_text + 100, 255, 255, 255);
    }

    if (curr_choose_text == medium_button_text)
    {
        draw_text(easy_button_text, x_first_button_text, y_first_button_text, 255, 255, 255);
        draw_text(">", x_first_button_text - 40, y_first_button_text + 50, 0, 255, 0);
        draw_text(medium_button_text, x_first_button_text, y_first_button_text + 50, 0, 255, 0);
        draw_text(hard_button_text, x_first_button_text, y_first_button_text + 100, 255, 255, 255);
    }

    if (curr_choose_text == hard_button_text)
    {
        draw_text(easy_button_text, x_first_button_text, y_first_button_text, 255, 255, 255);
        draw_text(medium_button_text, x_first_button_text, y_first_button_text + 50, 255, 255, 255);
        draw_text(">", x_first_button_text - 40, y_first_button_text + 100, 0, 255, 0);
        draw_text(hard_button_text, x_first_button_text, y_first_button_text + 100, 0, 255, 0);
    }
}

static void update(void)
{
    if (app.keyboard[SDL_SCANCODE_ESCAPE])
    {
        app.keyboard[SDL_SCANCODE_ESCAPE] = 0;
        game_settings.is_first_player_bot = SDL_FALSE;
        game_settings.is_second_player_bot = SDL_FALSE;
        game_settings.first_player_difficult = 0;
        game_settings.second_player_difficult = 0;

        init_menu();
    }

    if (app.keyboard[SDL_SCANCODE_RETURN])
    {
        app.keyboard[SDL_SCANCODE_RETURN] = 0;
        int choosen_difficult = 0;

        if (curr_choose_text == easy_button_text)
        {
            choosen_difficult = EASY;
        }

        else if (curr_choose_text == medium_button_text)
        {
            choosen_difficult = MEDIUM;
        }

        else if (curr_choose_text == hard_button_text)
        {
            choosen_difficult = HARD;
        }

        if (game_settings.is_first_player_bot)
        {

            if (!game_settings.first_player_difficult)
            {
                game_settings.first_player_difficult = choosen_difficult;
            }
            else
            {
                game_settings.second_player_difficult = choosen_difficult;
                init_shop();
            }
        }

        else
        {
            game_settings.second_player_difficult = choosen_difficult;
            init_shop();
        }
    }

    if (app.keyboard[SDL_SCANCODE_UP])
    {
        app.keyboard[SDL_SCANCODE_UP] = 0;

        if (curr_choose_text == easy_button_text)
        {
            curr_choose_text = hard_button_text;
        }

        else if (curr_choose_text == medium_button_text)
        {
            curr_choose_text = easy_button_text;
        }

        else if (curr_choose_text == hard_button_text)
        {
            curr_choose_text = medium_button_text;
        }
    }

    if (app.keyboard[SDL_SCANCODE_DOWN])
    {
        app.keyboard[SDL_SCANCODE_DOWN] = 0;

        if (curr_choose_text == easy_button_text)
        {
            curr_choose_text = medium_button_text;
        }

        else if (curr_choose_text == medium_button_text)
        {
            curr_choose_text = hard_button_text;
        }

        else if (curr_choose_text == hard_button_text)
        {
            curr_choose_text = easy_button_text;
        }
    }
}
