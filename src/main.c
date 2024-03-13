#include "main.h"

Uint32 deltaTime;

static void capFrameRate(Uint32 *then, Uint32 *deltaTime);

int main(void)
{
    memset(&app, 0, sizeof(App));

    init_SDL();

    atexit(cleanup);

    init_menu();

    Uint32 prevTime = 0;
    while (TRUE)
    {
        prepare_frame();

        handle_input();
        app.delegate.update();
        app.delegate.draw();

        present_frame();

        capFrameRate(&prevTime, &deltaTime);
    }

    return 0;
}

static void capFrameRate(Uint32 *then, Uint32 *deltaTime)
{
    long wait;

    wait = DELAY_TIME;

    *deltaTime = SDL_GetTicks() - *then;

    wait -= *deltaTime;

    if (wait < 1)
    {
        wait = 1;
    }

    SDL_Delay(wait);

    *then = SDL_GetTicks();
}