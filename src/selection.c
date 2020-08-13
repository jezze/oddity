#include <stdlib.h>
#include "define.h"
#include "box.h"
#include "list.h"
#include "widget.h"
#include "backend.h"
#include "view.h"
#include "selection.h"

struct list_item *selection_getclosest(struct selection *selection, unsigned int key)
{

    struct widget_area *a = selection->active->data;
    struct list_item *best = 0;
    int amx = a->size.x + a->size.w / 2;
    int amy = a->size.y + a->size.h / 2;
    int bestabsx = 5000;
    int bestabsy = 5000;
    struct list_item *current;

    for (current = selection->list.head; current; current = current->next)
    {

        struct widget_area *b = current->data;
        int bax = abs(b->size.x - a->size.x);
        int bay = abs(b->size.y - a->size.y);

        switch (key)
        {

        case KEY_LEFT:
            if (b->size.x + b->size.w >= amx)
                continue;

            if (bay <= bestabsy && bax < bestabsx)
            {

                best = current;
                bestabsx = bax;
                bestabsy = bay;

            }

            break;

        case KEY_RIGHT:
            if (b->size.x < amx)
                continue;

            if (bay <= bestabsy && bax < bestabsx)
            {

                best = current;
                bestabsx = bax;
                bestabsy = bay;

            }

            break;

        case KEY_UP:
            if (b->size.y + b->size.h >= amy)
                continue;

            if (bax <= bestabsx && bay < bestabsy)
            {

                best = current;
                bestabsx = bax;
                bestabsy = bay;

            }

            break;

        case KEY_DOWN:
            if (b->size.y < amy)
                continue;

            if (bax <= bestabsx && bay < bestabsy)
            {

                best = current;
                bestabsx = bax;
                bestabsy = bay;

            }

            break;

        }

    }

    return best;

}

struct list_item *selection_setclosest(struct selection *selection, unsigned int key)
{

    struct list_item *best = selection_getclosest(selection, key);

    if (best)
    {

        selection->active = best;

        backend_play("click");

    }

    return selection->active;

}

unsigned int selection_isactive(struct selection *selection, struct list_item *item)
{

    return selection->active == item;

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

void selection_return(struct selection *selection, unsigned int key, char *from)
{

    switch (key)
    {

    case KEY_B:
        view_quit(from);
        backend_play("click");

        break;

    }

}

