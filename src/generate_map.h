#ifndef GENERATE_MAP
#define GENERATE_MAP

#include <time.h>
#include <math.h>

#include "common.h"

void load(SDL_Window **, SDL_Renderer **, SDL_Rect *);
void generate_pixel_map();
int get_noise(double, double);
double noise_function(double, double);
void draw_pixel_map(void);
extern void init_menu(void);

extern App app;

#endif /* GENERATE_MAP */
