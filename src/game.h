#ifndef GAME
#define GAME
#define TANK_SIZE_W 60
#define TANK_SIZE_H 18
#define TANK_POWER 10
#define MUZZLE_LENGTH 30
#define MUZZLE_THICKNESS 3
#define MUZZLE_DEGREES 0
#define BULLET_W 10
#define BULLET_H 10
#define POWER 60
#define DELAY_POINTS 1000
#define DELAY_RADIUS 5
#define DELAY_HUMAN_INPUT 15
#define DELAY_BOT_INPUT 15
#define DELAY_BETWEEN_BULLET 100
#define SPEED_UP 3
#define TOTAL_MOVES TOTAL_WEAPONS / 2

#include "common.h"
#include "menu.h"
#include "map.h"
#include "draw.h"
#include "utils.h"
#include "leaderboard.h"
#include <SDL2/SDL2_gfxPrimitives.h>

typedef enum
{
    COLLISION_NONE,
    COLLISION_EARTH,
    COLLISION_TANK
} collision;

typedef enum
{
    DAMAGE_TARGET_NONE,
    DAMAGE_TARGET_OTHER,
    DAMAGE_TARGET_CURRENT,
    DAMAGE_TARGET_BOTH
} damage_target;

void init_game(ArsenalItem *left_arsenal, ArsenalItem *right_arsenal);

extern int pixel_map[SCREEN_WIDTH][SCREEN_HEIGHT];
#endif /* GAME */
