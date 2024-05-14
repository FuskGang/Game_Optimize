#include "menu.h"

static void update(void);
static void draw(void);
static void draw_cursor(void);
static void start(void);
static void about(void);
static void leaderboard(void);
static void quit(void);

static SDL_Texture *cursorTexture;

void init_menu(void)
{
	clear_widgets();

	Widget *w;
	int x;
	int y;

	x = 500;
	y = 200;

	w = create_widget("start");
	w->x = x;
	w->y = y + 0;
	strcpy(w->label, "Играть");
	w->action = &start;

	app.active_widget = w;

	w = create_widget("leaderboard");
	w->x = x;
	w->y = y + 50;
	strcpy(w->label, "Таблица лидеров");
	w->action = leaderboard;

	w = create_widget("help");
	w->x = x;
	w->y = y + 100;
	strcpy(w->label, "Справка");

	w = create_widget("about");
	w->x = x;
	w->y = y + 150;
	strcpy(w->label, "О программе");
	w->action = about;

	w = create_widget("exit");
	w->x = x;
	w->y = y + 200;
	strcpy(w->label, "Выход");
	w->action = quit;

	app.delegate.update = &update;
	app.delegate.draw = &draw;

	cursorTexture = load_texture("assets/cursor.png");
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
	blit(cursorTexture, &app.mouse, 1);
}

static void start(void)
{
	init_game();
}

static void about(void)
{
	init_about();
}

static void leaderboard(void)
{
	init_leaderboard();
}

static void quit(void)
{
	exit(0);
}
