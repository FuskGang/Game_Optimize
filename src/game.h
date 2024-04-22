#ifndef GAME
#define GAME

#include "common.h"
#include "menu.h"
#include "map.h"
#include "draw.h"
#include "utils.h"
#include <SDL2/SDL2_gfxPrimitives.h>

void init_game(void);

extern int pixel_map[SCREEN_WIDTH][SCREEN_HEIGHT];

#endif /* GAME */
