#ifndef STRUCTS
#define STRUCTS

#include <SDL2/SDL.h>
#include "constants.h"

typedef struct
{
	void (*update)(void);
	void (*draw)(void);
} Delegate;

typedef struct
{
	int start_x, end_x;
	int start_y, end_y;
	int thickness, length;
	int degrees;
	double angle;
} Muzzle;

typedef struct
{
	char player_name[255];
	SDL_Texture *texture;
	SDL_Color color;
	SDL_Rect size;
	Muzzle muzzle;
	int power;
	/*weapon list*/
} Tank;

typedef struct Widget
{
	char name[MAX_NAME_LENGTH];
	int x;
	int y;
	char label[MAX_NAME_LENGTH];
	struct Widget *prev;
	struct Widget *next;
	void (*action)(void);
} Widget;

typedef struct
{
	Delegate delegate;
	SDL_Renderer *renderer;
	SDL_Window *window;
	SDL_Rect mouse;
	int keyboard[MAX_KEYBOARD_KEYS];
	Widget *active_widget;
} App;

#endif /* STRUCTS */
