#include <stdlib.h>
#include "define.h"
#include "box.h"
#include "list.h"
#include "widget.h"
#include "backend.h"
#include "view.h"
#include "selection.h"

struct widget *selection_getclosest(struct selection *selection, unsigned int key)
{

    struct widget *a;
    struct widget *best = 0;
    struct list_item *current;
    int bestabsx = 5000;
    int bestabsy = 5000;
    int amx;
    int amy;

    if (!selection->active)
        return 0;

    a = selection->active;
    amx = a->size.x + a->size.w / 2;
    amy = a->size.y + a->size.h / 2;

    for (current = selection->list.head; current; current = current->next)
    {

        struct widget *b = current->data;
        int bax = abs(b->size.x - a->size.x);
        int bay = abs(b->size.y - a->size.y);

        switch (key)
        {

        case KEY_LEFT:
            if (b->size.x + b->size.w >= amx)
                continue;

            if (bay <= bestabsy && bax < bestabsx)
            {

                best = b;
                bestabsx = bax;
                bestabsy = bay;

            }

            break;

        case KEY_RIGHT:
            if (b->size.x < amx)
                continue;

            if (bay <= bestabsy && bax < bestabsx)
            {

                best = b;
                bestabsx = bax;
                bestabsy = bay;

            }

            break;

        case KEY_UP:
            if (b->size.y + b->size.h >= amy)
                continue;

            if (bax <= bestabsx && bay < bestabsy)
            {

                best = b;
                bestabsx = bax;
                bestabsy = bay;

            }

            break;

        case KEY_DOWN:
            if (b->size.y < amy)
                continue;

            if (bax <= bestabsx && bay < bestabsy)
            {

                best = b;
                bestabsx = bax;
                bestabsy = bay;

            }

            break;

        }

    }

    return best;

}

struct widget *selection_setclosest(struct selection *selection, unsigned int key)
{

    struct widget *best = selection_getclosest(selection, key);

    if (best)
    {

        selection->active = best;

        backend_play("click");

    }

    return selection->active;

}

unsigned int selection_isactive(struct selection *selection, struct widget *widget)
{

    return selection->active == widget;

}

void selection_select(struct selection *selection, unsigned int key, char *from, char *to)
{

    switch (key)
    {

    case KEY_A:
        view_load(to, from);
        backend_play("select");

        break;

    }

}

void selection_unselect(struct selection *selection, unsigned int key, char *from)
{

    switch (key)
    {

    case KEY_B:
        view_quit(from);
        backend_play("unselect");

        break;

    }

}

void selection_add(struct selection *selection, struct widget *widget)
{

    list_add(&selection->list, &widget->item);

}

void selection_render(struct selection *selection, unsigned int ticks)
{

    if (selection->active)
    {

        struct widget *widget = selection->active;

        backend_paint_selection(widget->size.x, widget->size.y, widget->size.w, widget->size.h);

    }

}

void selection_reset(struct selection *selection)
{

    if (selection->list.head)
        selection->active = selection->list.head->data;
    else
        selection->active = 0;

}

