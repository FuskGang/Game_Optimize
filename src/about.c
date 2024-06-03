#include "about.h"

static void draw(void);
static void update(void);

void init_about(void)
{
    app.delegate.draw = draw;
    app.delegate.update = update;
}

void draw(void)
{
    draw_text("Создатели: Барабанов Илья Витальевич, Гранкин Глеб Олегович", 100, 100, 255, 255, 255);
    draw_text("Группа: 5151003/30002", 100, 120, 255, 255, 255);
    draw_text("Год: 2024", 100, 140, 255, 255, 255);
    draw_text("ВУЗ: Санкт-Петербургский политехнический университет Петра Великого", 100, 160, 255, 255, 255);
    draw_text("Институт: Институт компьютерных наук и кибербезопасности, Высшая школа кибербезопасности", 100, 180, 255, 255, 255);
}

void update(void)
{
    if (app.keyboard[SDL_SCANCODE_ESCAPE])
    {
        app.keyboard[SDL_SCANCODE_ESCAPE] = 0;
        init_menu();
    }
}