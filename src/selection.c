#include <stdlib.h>
#include <string.h>
#include "define.h"
#include "box.h"
#include "list.h"
#include "widget.h"
#include "selection.h"
#include "view.h"
#include "backend.h"
#include "main.h"

unsigned int selection_isactive(struct selection *selection, char *id)
{

    if (!selection->active)
        return 0;

    return !strcmp(selection->active->id, id);

}

void selection_move(struct selection *selection, unsigned int key)
{

    struct widget *a;
    struct widget *best = 0;
    struct list_item *current;
    int bestabsx = 5000;
    int bestabsy = 5000;
    int amx;
    int amy;

    if (!selection->active)
        return;

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

    if (best)
    {

        selection->active = best;

        backend_play("click");

    }

}

void selection_select(struct selection *selection, unsigned int key, char *match, char *from, char *to)
{

    if (key != KEY_A)
        return;

    if (!selection->active)
        return;

    if (!strlen(selection->active->id))
        return;

    if (strcmp(selection->active->id, match))
        return;

    main_loadview(to, from);
    backend_play("select");

}

void selection_unselect(struct selection *selection, unsigned int key, char *from)
{

    if (key != KEY_B)
        return;

    main_quitview(from);
    backend_play("unselect");

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

