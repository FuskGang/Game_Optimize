#include "main.h"

App app;
Uint32 deltaTime;
int frameCount = 0;
Uint32 lastTime;

static void cap_frame_rate(Uint32 *then, Uint32 *deltaTime);

int main(__attribute__((unused)) int argc, __attribute__((unused)) char *argv[])
{
    memset(&app, 0, sizeof(App));

    init_SDL();

    atexit(cleanup);

    init_fonts();

    init_widgets();

    init_menu();

    Uint32 prevTime = 0;
    lastTime = SDL_GetTicks();
    while (TRUE)
    {
        prepare_frame();

        handle_input();
        app.delegate.update();
        app.delegate.draw();

        present_frame();

        cap_frame_rate(&prevTime, &deltaTime);
    }

    return 0;
}

static void cap_frame_rate(Uint32 *then, Uint32 *deltaTime)
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

    Uint32 currentTime = SDL_GetTicks();
    frameCount++;
    if (currentTime > lastTime + 1000)
    {
        app.fps = frameCount * 1000.0f / (currentTime - lastTime);
        lastTime = currentTime;
        frameCount = 0;
    }

    *then = SDL_GetTicks();
}
