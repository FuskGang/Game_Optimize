#ifndef DRAW
#define DRAW

#include "common.h"

#include <SDL2/SDL_image.h>

void prepare_frame(void);
void present_frame(void);
SDL_Texture *load_texture(char *filename);
SDL_Surface *load_surface(char *filename);
void blit(SDL_Texture *texture, SDL_Rect dest, int center);

#endif /* DRAW */
