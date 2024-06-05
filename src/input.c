#include "input.h"

static void do_key_up(SDL_KeyboardEvent *event)
{
    if (event->repeat == 0 && event->keysym.scancode < MAX_KEYBOARD_KEYS)
    {
        app.keyboard[event->keysym.scancode] = 0;
    }
}

static void do_key_down(SDL_KeyboardEvent *event)
{
    if (event->repeat == 0 && event->keysym.scancode < MAX_KEYBOARD_KEYS)
    {
        app.keyboard[event->keysym.scancode] = 1;
    }
}

static void do_textinput(char input_text[][32])
{
    app.last_input_sym = input_text[0][0];
}

void handle_input(void)
{
    SDL_GetMouseState(&app.mouse.x, &app.mouse.y);

    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            exit(0);
            break;

        case SDL_KEYDOWN:
            do_key_down(&event.key);
            break;

        case SDL_KEYUP:
            do_key_up(&event.key);
            break;

        case SDL_TEXTINPUT:
            do_textinput(&event.text.text);
            break;

        default:
            break;
        }
    }
}
