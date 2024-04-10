#ifndef MAP
#define MAP

#include <time.h>

#include "common.h"

extern int pixel_map[SCREEN_WIDTH][SCREEN_HEIGHT];

void draw_pixel_map(void);
void init_map(void);

#endif /* MAP */
