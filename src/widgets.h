#ifndef WIDGETS
#define WIDGETS

#include "common.h"
#include "text.h"

void init_widgets(void);
Widget *create_widget(char *name);
void do_widgets(void);
void draw_widgets(void);
void clear_widgets(void);

#endif /* WIDGETS */
