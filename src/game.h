#ifndef GAME
#define GAME
#define MAX_BULLET 5
#define TANK_SIZE_W 60
#define TANK_SIZE_H 18
#define TANK_POWER 10
#define MUZZLE_LENGTH 30
#define MUZZLE_THICKNESS 3
#define MUZZLE_DEGREES 0
#define BULLET_W 10
#define BULLET_H 10
#define POWER 60

#include "common.h"
#include "menu.h"
#include "map.h"
#include "draw.h"
#include "utils.h"
#include <SDL2/SDL2_gfxPrimitives.h>

typedef enum
{
    COLLISION_NONE,
    COLLISION_EARTH,
    COLLISION_TANK
} collision;

void init_game(void);

extern int pixel_map[SCREEN_WIDTH][SCREEN_HEIGHT];
#endif /* GAME */
