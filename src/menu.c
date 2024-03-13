#include "menu.h"

static void update(void);
static void draw(void);
static void update_user(void);

static SDL_Texture *cursorTexture;

void init_menu(void)
{
    app.delegate.update = update;
    app.delegate.draw = draw;

    cursorTexture = load_texture("assets/alien.png");
}

static void update(void)
{
    update_user();
}

static void draw(void)
{
    blit(cursorTexture, app.mouse.x, app.mouse.y, 1);
}

static void update_user(void)
{
}