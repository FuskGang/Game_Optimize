#include "about.h"

static void draw(void);
static void update(void);

void init_about(void)
{
    app.delegate.draw = draw;
    app.delegate.update = update;
}

void draw(void)
{
    draw_text("Мы сделали эту игру!", 100, 100, 255, 255, 255);
}


void update(void)
{
    if (app.keyboard[SDL_SCANCODE_ESCAPE])
    {
        app.keyboard[SDL_SCANCODE_ESCAPE] = 0;
        init_menu();
    }
}