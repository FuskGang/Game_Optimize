#include "utils.h"

int get_angle(int x1, int y1, int x2, int y2)
{
    int angle = -90 + atan2(y1 - y2, x1 - x2) * (180 / M_PI);
    return angle >= 0 ? angle : 360 + angle;
}

void rotate_point(int x, int y, int cx, int cy, int degrees, int *new_x, int *new_y)
{
    double radians = degrees * M_PI / 180.0;
    *new_x = cos(radians) * (x - cx) - sin(radians) * (y - cy) + cx;
    *new_y = sin(radians) * (x - cx) + cos(radians) * (y - cy) + cy;
}
