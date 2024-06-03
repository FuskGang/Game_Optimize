#include "main.h"

App app;
GameSettings game_settings;
Uint32 delta_time;
int frame_count = 0;
Uint32 last_time;

static void cap_frame_rate(Uint32 *then, Uint32 *deltaTime);

int main(__attribute__((unused)) int argc, __attribute__((unused)) char *argv[])
{
    memset(&app, 0, sizeof(App));
    memset(&game_settings, 0, sizeof(GameSettings));

    init_SDL();

    atexit(cleanup);

    init_fonts();

    init_widgets();
    
    init_menu();

    Uint32 prev_time = 0;
    last_time = SDL_GetTicks();
    while (TRUE)
    {
        prepare_frame();

        handle_input();
        app.delegate.update();
        app.delegate.draw();

        present_frame();

        cap_frame_rate(&prev_time, &delta_time);
    }

    return 0;
}

static void cap_frame_rate(Uint32 *then, Uint32 *delta_time)
{
    long wait;

    wait = DELAY_TIME;

    *delta_time = SDL_GetTicks() - *then;

    wait -= *delta_time;

    if (wait < 1)
    {
        wait = 1;
    }

    SDL_Delay(wait);

    Uint32 current_time = SDL_GetTicks();
    frame_count++;
    if (current_time > last_time + 1000)
    {
        app.fps = frame_count * 1000.0f / (current_time - last_time);
        last_time = current_time;
        frame_count = 0;
    }

    *then = SDL_GetTicks();
}
