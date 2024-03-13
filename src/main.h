#ifndef MAIN
#define MAIN

#include "common.h"

extern void cleanup(void);
extern void handle_input(void);
extern void init_SDL(void);
extern void init_menu(void);
extern void prepare_frame(void);
extern void present_frame(void);

App app;

#endif /* MAIN */
