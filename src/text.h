#ifndef TEXT
#define TEXT

#include <SDL2/SDL_ttf.h>

#include "common.h"
#include "textures.h"

extern SDL_Texture *to_texture(SDL_Surface *surface, int destroySurface);

#endif /* TEXT */
