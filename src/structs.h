#ifndef STRUCTS
#define STRUCTS

#include <SDL2/SDL.h>
#include "constants.h"

typedef struct {
    void (*update)(void);
    void (*draw)(void);
} Delegate;

typedef struct {
	int x;
	int y;
} Mouse;

typedef struct
{
	SDL_Renderer *renderer;
	SDL_Window *window;
	Delegate delegate;
    Mouse mouse;
    int keyboard[MAX_KEYBOARD_KEYS];
} App;

#endif /* STRUCTS */
