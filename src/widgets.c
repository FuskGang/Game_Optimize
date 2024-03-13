#include "widgets.h"

static Widget widgetHead;
static Widget *widgetTail;

void init_widgets(void)
{
	memset(&widgetHead, 0, sizeof(Widget));
	widgetTail = &widgetHead;
}

Widget *create_widget(char *name)
{
	Widget *w;

	w = (Widget *)malloc(sizeof(Widget));
	memset(w, 0, sizeof(Widget));
	widgetTail->next = w;
	w->prev = widgetTail;
	widgetTail = w;

	strcpy(w->name, name);

	return w;
}

void do_widgets(void)
{
	if (app.keyboard[SDL_SCANCODE_UP])
	{
		app.keyboard[SDL_SCANCODE_UP] = 0;

		app.active_widget = app.active_widget->prev;

		if (app.active_widget == &widgetHead)
		{
			app.active_widget = widgetTail;
		}
	}

	if (app.keyboard[SDL_SCANCODE_DOWN])
	{
		app.keyboard[SDL_SCANCODE_DOWN] = 0;

		app.active_widget = app.active_widget->next;

		if (app.active_widget == NULL)
		{
			app.active_widget = widgetHead.next;
		}
	}

	if (app.keyboard[SDL_SCANCODE_SPACE] || app.keyboard[SDL_SCANCODE_RETURN])
	{
		if (app.active_widget->action != NULL)
		{
			app.active_widget->action();
		}
	}
}

void draw_widgets(void)
{
	Widget *w;
	SDL_Color c;

	for (w = widgetHead.next; w != NULL; w = w->next)
	{
		if (w == app.active_widget)
		{
			c.g = 255;
			c.r = c.b = 0;

			draw_text(">", w->x - 40, w->y, c.r, c.g, c.b);
		}
		else
		{
			c.r = c.g = c.b = 255;
		}

		draw_text(w->label, w->x, w->y, c.r, c.g, c.b);
	}
}

void clear_widgets(void)
{
    Widget *current = widgetHead.next;
    Widget *next;

    while (current != NULL)
    {
        next = current->next;
        free(current);
        current = next;
    }

    widgetHead.next = NULL;
    widgetTail = &widgetHead;
}