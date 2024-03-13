#include "menu.h"

static void update(void);
static void draw(void);
static void draw_cursor(void);

static SDL_Texture *cursorTexture;

void init_menu(void)
{
    Widget *w;
	int x;

	x = 500;

	w = create_widget("start");
	w->x = x;
	w->y = 200;
	strcpy(w->label, "Start");

	app.active_widget = w;

	w = create_widget("options");
	w->x = x;
	w->y = 250;
	strcpy(w->label, "Options");

	w = create_widget("credits");
	w->x = x;
	w->y = 300;
	strcpy(w->label, "Credits");

	w = create_widget("exit");
	w->x = x;
	w->y = 350;
	strcpy(w->label, "Exit");

	app.delegate.update = &update;
	app.delegate.draw = &draw;


    cursorTexture = load_texture("assets/alien.png");
}

static void update(void)
{
    do_widgets();
}

static void draw(void)
{
    draw_cursor();
    draw_widgets();
}

void draw_cursor(void)
{
    blit(cursorTexture, app.mouse.x, app.mouse.y, 1);
}
