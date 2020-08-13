#include <stdlib.h>
#include "define.h"
#include "box.h"
#include "list.h"
#include "widget.h"

struct selection
{

    struct list list;
    struct list_item *active;

};

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
        selection->active = best;

    return selection->active;

}

