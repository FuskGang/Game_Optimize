#ifndef UTILS
#define UTILS

#include <math.h>
#include "constants.h"

int get_angle(int x1, int y1, int x2, int y2);
void rotate_point(int x, int y, int cx, int cy, int degrees, int *new_x, int *new_y);

#endif /* UTILS */
