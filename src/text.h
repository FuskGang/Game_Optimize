#ifndef TEXT
#define TEXT

#include <SDL2/SDL_ttf.h>

#include "common.h"
#include "textures.h"

void init_fonts(void);
void draw_text(char *text, int x, int y, int r, int g, int b);

#endif /* TEXT */
