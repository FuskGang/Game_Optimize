#include "utils.h"

int get_angle(int x1, int y1, int x2, int y2)
{
    int angle = -90 + atan2(y1 - y2, x1 - x2) * (180 / M_PI);
    return angle >= 0 ? angle : 360 + angle;
}