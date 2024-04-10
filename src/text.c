#include "text.h"

static SDL_Texture *get_text_texture(char *text, int *w, int *h);

static TTF_Font *font;
static SDL_Color white = {255, 255, 255, 255};

void init_fonts(void)
{
	font = TTF_OpenFont("assets/SourceCodePro-Regular.ttf", FONT_SIZE);
}

static SDL_Texture *get_text_texture(char *text, int *w, int *h)
{
	SDL_Surface *surface;

	surface = TTF_RenderUTF8_Blended(font, text, white);

	if (w != NULL)
		*w = surface->w;
	if (h != NULL)
		*h = surface->h;

	return to_texture(surface, 1);
}

void draw_text(char *text, int x, int y, int r, int g, int b)
{
	SDL_Rect dest = {x, y, 0, 0};
	SDL_Texture *text_texture = get_text_texture(text, &dest.w, &dest.h);
	SDL_SetTextureColorMod(text_texture, r, g, b);
	SDL_RenderCopy(app.renderer, text_texture, NULL, &dest);
}
