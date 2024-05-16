#ifndef LEADERBOARD
#define LEADERBOARD

#include "common.h"
#include "menu.h"
#define MAX_PLAYERS 10
#define MAX_LINE_LENGTH MAX_NAME_LENGTH + 10

void init_leaderboard(void);
void update_leaderboard(int, char *);

typedef struct
{
    int points;
    char name[MAX_NAME_LENGTH];
} LeaderboardEntry;

#endif /* ABOUT */
