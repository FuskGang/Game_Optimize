#include "choose_players_name.h"

static void draw(void);
static void update(void);

static char first_player_temp_name[MAX_NAME_LENGTH];
static char second_player_temp_name[MAX_NAME_LENGTH];
static char *curr_player_temp_name;
static int curr_player_name_ind;
Uint32 input_time;

void init_choose_name(void)
{
    app.delegate.draw = draw;
    app.delegate.update = update;
    curr_player_temp_name = first_player_temp_name;
    curr_player_name_ind = 0;
    app.last_input_sym = 0;
    SDL_StartTextInput();
}

static void draw(void)
{
    if (curr_player_temp_name[0] == '\0')
    {
        if (curr_player_temp_name == first_player_temp_name)
        {
            draw_text("Введите имя первого игрока: ", 220, 200, 255, 255, 255);
        }
        else
        {
            draw_text("Введите имя второго игрока: ", 220, 200, 255, 255, 255);
        }
    }
    else
    {
        if (curr_player_temp_name == first_player_temp_name)
        {
            draw_text("Введите имя первого игрока: ", 220, 200, 255, 255, 255);
        }
        else
        {
            draw_text("Введите имя второго игрока: ", 220, 200, 255, 255, 255);
        }

        draw_text(curr_player_temp_name, 500, 200, 255, 255, 255);
    }
}

static void update(void)
{
    if ((SDL_GetTicks() - input_time) < DELAY_INPUT)
    {
        return;
    }

    input_time = SDL_GetTicks();

    if (app.keyboard[SDL_SCANCODE_ESCAPE])
    {
        app.keyboard[SDL_SCANCODE_ESCAPE] = 0;
        game_settings.is_first_player_bot = SDL_FALSE;
        game_settings.is_second_player_bot = SDL_FALSE;
        game_settings.first_player_difficult = 0;
        game_settings.second_player_difficult = 0;

        SDL_StopTextInput();
        init_menu();
    }

    if (app.keyboard[SDL_SCANCODE_RETURN])
    {
        app.keyboard[SDL_SCANCODE_RETURN] = 0;
        curr_player_temp_name[curr_player_name_ind] = '\0';
        curr_player_name_ind = 0;


        if (curr_player_temp_name == first_player_temp_name)
        {
            strcpy(game_settings.first_player_name, first_player_temp_name);
            curr_player_temp_name = second_player_temp_name;
        }

        else
        {
            strcpy(game_settings.first_player_name, second_player_temp_name);
            SDL_StopTextInput();
            init_shop();
        }
    }

    if (app.keyboard[SDL_SCANCODE_BACKSPACE])
    {
        if (curr_player_name_ind > 0)
        {
            curr_player_temp_name[curr_player_name_ind--] = '\0';
        }

        if (curr_player_name_ind == 0)
        {
            curr_player_temp_name[0] = '\0';
        }
    }

    if (curr_player_name_ind < MAX_NAME_LENGTH)
    {
        if (app.last_input_sym != 0)
        {
            curr_player_temp_name[curr_player_name_ind++] = app.last_input_sym;
            app.last_input_sym = 0;
        }
    }
}
