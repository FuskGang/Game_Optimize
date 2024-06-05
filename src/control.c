#include "control.h"

static void draw(void);
static void update(void);

void init_control(void)
{
    app.delegate.draw = draw;
    app.delegate.update = update;
}

void draw(void)
{
    draw_text("Управление реализовано с помощью следующих клавиш: ", 100, 100, 255, 255, 255);
    draw_text("Верхние клавиши - 1, 2, 3. С их помощью можно менять оружия", 100, 120, 255, 255, 255);
    draw_text("На стрелки влево и вправо - происходит поворот оружия", 100, 140, 255, 255, 255);
    draw_text("С помощью пробела - происходит выстрел", 100, 160, 255, 255, 255);
    draw_text("На клавишу z - танк едет влево, а на клавишу x - вправо", 100, 180, 255, 255, 255);
}


void update(void)
{
    if (app.keyboard[SDL_SCANCODE_ESCAPE])
    {
        app.keyboard[SDL_SCANCODE_ESCAPE] = 0;
        init_menu();
    }
}