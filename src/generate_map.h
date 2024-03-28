#ifndef GENERATE_MAP
#define GENERATE_MAP

#define _USE_MATH_DEFINES

#include <time.h>
#include <math.h>

#include "common.h"

void load(SDL_Window **, SDL_Renderer **, SDL_Rect *);
void generate_pixel_map();
void draw_shoot_info();
void draw_text(char *text, int x, int y, int r, int g, int b);
int get_noise(double, double);
double noise_function(double, double);
void draw_pixel_map(void);
void fire(Bullet *b, float x, float y, float angle, float velocity);
void update_pos_b(Bullet *b, float time_wased, float angle);
int render_b(Bullet *b);
extern SDL_Texture *load_texture(char *filename);
extern void blit(SDL_Texture *texture, int x, int y, int center);
extern void init_menu(void);

extern App app;

#endif /* GENERATE_MAP */
