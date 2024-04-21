#include "menu.h"

static void update(void);
static void draw(void);
static void draw_cursor(void);
static void start(void);
static void quit(void);

static SDL_Texture *cursorTexture;

void init_menu(void)
{
	clear_widgets();

    Widget *w;
	int x;

	x = 500;

	w = create_widget("start");
	w->x = x;
	w->y = 200;
	strcpy(w->label, "Start");
	w->action = &start;

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
	w->action = quit;

	app.delegate.update = &update;
	app.delegate.draw = &draw;


    cursorTexture = load_texture("assets/alien.png");
	SDL_QueryTexture(cursorTexture, NULL, NULL, &app.mouse.w, &app.mouse.h);
}

static void update(void)
{
    do_widgets();
}

static void draw(void)
{
    draw_widgets();
	draw_cursor();
}

void draw_cursor(void)
{
    blit(cursorTexture, app.mouse, 1);
}

static void start(void)
{
	init_game();
}

static void quit(void)
{
	exit(0);
}
