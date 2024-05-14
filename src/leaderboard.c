#include "leaderboard.h"

static void draw(void);
static void update(void);
int read_leaderboard_file(FILE *i_stream, int *point, char *name);
void get_leaderboard();
void sort_leaderboard();
int *leaderboard_points = NULL;
char leaderboard_names[MAX_PLAYERS][MAX_NAME_LENGTH];

void init_leaderboard(void)
{
    app.delegate.draw = draw;
    app.delegate.update = update;
}

void get_leaderboard()
{
    FILE *leader_file_stream = fopen("assets/leaderboard.txt", "r");
    leaderboard_points = (int *)(malloc(sizeof(int) * MAX_PLAYERS));

    if (leader_file_stream == NULL)
    {
        for (int i = 0; i < MAX_PLAYERS; i++)
        {
            leaderboard_points[i] = -1;
            strcpy(leaderboard_names[i], "NONE");
        }

        return;
    }

    int read_row_cnt = 0;
    int code = 0;
    int curr_point = 0;
    char curr_name[MAX_NAME_LENGTH];

    while (1)
    {
        code = read_leaderboard_file(leader_file_stream, &curr_point, curr_name);

        leaderboard_points[read_row_cnt] = curr_point;
        strcpy(leaderboard_names[read_row_cnt], curr_name);

        read_row_cnt++;
        curr_point = 0;

        if (code == 0)
        {
            break;
        }
    }

    while (read_row_cnt != MAX_PLAYERS)
    {
        leaderboard_points[read_row_cnt++] = -1;
    }

    fclose(leader_file_stream);
}

void sort_leaderboard()
{
    int tmp_point = 0;
    char *tmp_name = (char *)(malloc(sizeof(char) * MAX_NAME_LENGTH));
    char *tmp_name_rpl = (char *)(malloc(sizeof(char) * MAX_NAME_LENGTH));
    int i, j;

    for (int dist = MAX_PLAYERS / 2; dist > 0; dist /= 2)
    {
        for (i = dist; i < MAX_PLAYERS; i++)
        {
            tmp_point = leaderboard_points[i];
            strcpy(tmp_name, leaderboard_names[i]);

            for (j = i; j >= dist && leaderboard_points[j - dist] > tmp_point; j -= dist)
            {
                leaderboard_points[j] = leaderboard_points[j - dist];
                strcpy(leaderboard_names[j], leaderboard_names[j - dist]);
            }

            leaderboard_points[j] = tmp_point;
            strcpy(leaderboard_names[j], tmp_name);
        }
    }

    free(tmp_name);
    free(tmp_name_rpl);
}

void update_leaderboard(int point, char *name)
{
    get_leaderboard();
    sort_leaderboard();

    if (point > leaderboard_points[0])
    {
        leaderboard_points[0] = point;

        strcpy(leaderboard_names[0], name);

        sort_leaderboard();

        FILE *leaderboard_file_stream = fopen("assets/leaderboard.txt", "w");

        for (int i = 0; i < MAX_PLAYERS; i++)
        {
            if (i == 0)
            {
                fprintf(leaderboard_file_stream, "%s:%d", leaderboard_names[i], leaderboard_points[i]);
            }
            else
            {
                fprintf(leaderboard_file_stream, "\n%s:%d", leaderboard_names[i], leaderboard_points[i]);
            }
        }
    }
}

int read_leaderboard_file(FILE *i_stream, int *point, char *name)
{
    int sym;
    int read_sym_cnt = 0;

    while (1)
    {
        sym = fgetc(i_stream);

        if (sym == ':')
        {
            break;
        }

        name[read_sym_cnt++] = sym;

        if (read_sym_cnt == MAX_NAME_LENGTH)
        {
            break;
        }
    }

    name[read_sym_cnt] = 0;
    read_sym_cnt = 0;

    while (1)
    {
        sym = fgetc(i_stream);

        if (sym == EOF || sym == '\n')
        {
            break;
        }

        (*point) = (*point * 10) + (sym - '0');
        read_sym_cnt++;
    }

    if (sym == EOF)
    {
        return 0;
    }

    return 1;
}

void draw(void)
{
    char player_stat[MAX_NAME_LENGTH + 20];

    for (int cnt_player = 0; cnt_player < MAX_PLAYERS; cnt_player++)
    {
        if (leaderboard_points != NULL && leaderboard_points[MAX_PLAYERS - cnt_player - 1] != -1)
        {
            sprintf(player_stat, "%d) %s - %d", cnt_player + 1, leaderboard_names[MAX_PLAYERS - cnt_player - 1], leaderboard_points[MAX_PLAYERS - cnt_player - 1]);
        }
        else
        {
            sprintf(player_stat, "%d) NONE  - NONE", cnt_player + 1);
        }

        draw_text(player_stat, 100, 100 + 20 * cnt_player, 255, 255, 255);

    }
}

void update(void)
{
    if (leaderboard_points == NULL)
    {
        get_leaderboard();
        sort_leaderboard();
    }

    if (app.keyboard[SDL_SCANCODE_ESCAPE])
    {
        app.keyboard[SDL_SCANCODE_ESCAPE] = 0;

        free(leaderboard_points);
        leaderboard_points = NULL;
        init_menu();
    }
}
