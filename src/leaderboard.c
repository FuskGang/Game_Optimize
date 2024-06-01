#include "leaderboard.h"

static void draw(void);
static void update(void);
static void fill_leaderboard();

LeaderboardEntry *leaderboard_players = NULL;
int count_leaderboard_players;

void init_leaderboard(void)
{
    app.delegate.draw = draw;
    app.delegate.update = update;
}

static int compare_player_points(const void *p1, const void *p2)
{
    return (((LeaderboardEntry *)p1)->points - ((LeaderboardEntry *)p2)->points);
}

void update_leaderboard(int points, char *name)
{
    fill_leaderboard();

    leaderboard_players = (LeaderboardEntry *)(realloc(leaderboard_players, sizeof(LeaderboardEntry) * (count_leaderboard_players + 1)));

    leaderboard_players[count_leaderboard_players].points = points;
    strcpy(leaderboard_players[count_leaderboard_players++].name, name);

    FILE *leader_file_stream = fopen("leaderboard.txt", "w");
    qsort((void *)leaderboard_players, count_leaderboard_players, sizeof(LeaderboardEntry), compare_player_points);

    for (int index_curr_player = 0; index_curr_player < SDL_min(count_leaderboard_players, MAX_PLAYERS); index_curr_player++)
    {
        fprintf(leader_file_stream, "%s:%d\n", leaderboard_players[count_leaderboard_players - index_curr_player - 1].name, leaderboard_players[count_leaderboard_players - index_curr_player - 1].points);
    }

    printf("%d\n", count_leaderboard_players);

    fclose(leader_file_stream);
    free(leaderboard_players);
    leaderboard_players = NULL;
    count_leaderboard_players = 0;
}

static void fill_leaderboard()
{
    FILE *leader_file_stream = fopen("leaderboard.txt", "r");

    if (leader_file_stream == NULL)
    {
        return;
    }

    char read_line[MAX_LINE_LENGTH];
    char player_name[MAX_NAME_LENGTH];
    int player_point;

    while (fgets(read_line, sizeof(read_line), leader_file_stream))
    {
        if (sscanf(read_line, "%255[^:]:%d", player_name, &player_point) == 2)
        {
            leaderboard_players = (LeaderboardEntry *)(realloc(leaderboard_players, sizeof(LeaderboardEntry) * (count_leaderboard_players + 1)));
            leaderboard_players[count_leaderboard_players].points = player_point;
            strcpy(leaderboard_players[count_leaderboard_players++].name, player_name);
        }
        else
        {
            break;
        }
    }

    fclose(leader_file_stream);
}

static void draw(void)
{
    char players_stat[MAX_LINE_LENGTH + 20];

    if (!count_leaderboard_players)
    {
        sprintf(players_stat, "В данный момент таблица лидеров пуста");
        draw_text(players_stat, 100, 100, 255, 255, 255);
    }

    for (int index_curr_player = 0; index_curr_player < count_leaderboard_players; index_curr_player++)
    {
        sprintf(players_stat, "%d) %s - %d point", index_curr_player + 1, leaderboard_players[count_leaderboard_players - index_curr_player - 1].name, leaderboard_players[count_leaderboard_players - index_curr_player - 1].points);
        draw_text(players_stat, 100, 100 + 20 * index_curr_player, 255, 255, 255);
    }
}

static void update(void)
{
    if (leaderboard_players == NULL)
    {
        fill_leaderboard();
        qsort((void *)leaderboard_players, count_leaderboard_players, sizeof(LeaderboardEntry), compare_player_points);
    }

    if (app.keyboard[SDL_SCANCODE_ESCAPE])
    {
        app.keyboard[SDL_SCANCODE_ESCAPE] = 0;

        free(leaderboard_players);

        leaderboard_players = NULL;
        count_leaderboard_players = 0;

        init_menu();
    }
}
