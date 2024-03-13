#ifndef MENU
#define MENU

#include "common.h"
#include "text.h"

extern void blit(SDL_Texture *texture, int x, int y, int center);
extern SDL_Texture *load_texture(char *filename);
extern SDL_Texture *get_text_texture(char *text, int *w, int *h);
extern void draw_text(char *text, int x, int y, int r, int g, int b);
extern void do_widgets(void);
extern void draw_widgets(void);
extern Widget *create_widget(char *name);
extern void init_map(void);
extern void clear_widgets(void);

extern App app;

#endif /* MENU */
