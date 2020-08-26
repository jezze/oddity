#include <stdlib.h>
#include <string.h>
#include "define.h"
#include "box.h"
#include "list.h"
#include "widget.h"
#include "view.h"
#include "backend.h"
#include "main.h"

static void placewidget(struct widget *widget, struct box *size)
{

    switch (widget->type)
    {

    case WIDGET_TYPE_AREA:
        widget_area_place(widget, size);

        break;

    case WIDGET_TYPE_SLIDER:
        widget_slider_place(widget, size);

        break;

    case WIDGET_TYPE_TEXT:
        widget_text_place(widget, size);

        break;

    }

}

static void renderwidget(struct widget *widget, unsigned int ticks)
{

    switch (widget->type)
    {

    case WIDGET_TYPE_AREA:
        widget_area_render(widget, ticks);

        break;

    case WIDGET_TYPE_SLIDER:
        widget_slider_render(widget, ticks);

        break;

    case WIDGET_TYPE_TEXT:
        widget_text_render(widget, ticks);

        break;

    }

}

void view_place(struct view *view, struct widget *parent)
{

    struct list_item *current;

    for (current = view->widgets.head; current; current = current->next)
    {

        struct widget *widget = current->data;

        if (strlen(parent->id) && !strcmp(widget->in, parent->id))
        {

            placewidget(widget, &parent->size);
            view_place(view, widget);

        }

    }

}

void view_render(struct view *view, struct widget *parent, unsigned int ticks)
{

    struct list_item *current;

    for (current = view->widgets.head; current; current = current->next)
    {

        struct widget *widget = current->data;

        if (strlen(parent->id) && !strcmp(widget->in, parent->id))
        {

            renderwidget(widget, ticks);
            view_render(view, widget, ticks);

        }

    }

}

unsigned int view_isactive(struct view *view, char *id)
{

    if (!view->selection.active)
        return 0;

    return !strcmp(view->selection.active->id, id);

}

void view_moveselection(struct view *view, unsigned int key)
{

    struct widget *a;
    struct widget *best = 0;
    struct list_item *current;
    int bestabsx = 5000;
    int bestabsy = 5000;
    int amx;
    int amy;

    if (!view->selection.active)
        return;

    a = view->selection.active;
    amx = a->size.x + a->size.w / 2;
    amy = a->size.y + a->size.h / 2;

    for (current = view->selection.list.head; current; current = current->next)
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

        view->selection.active = best;

        backend_play("click");

    }

}

void view_select(struct view *view, unsigned int key, char *match, char *from, char *to)
{

    if (key != KEY_A)
        return;

    if (!view->selection.active)
        return;

    if (!strlen(view->selection.active->id))
        return;

    if (strcmp(view->selection.active->id, match))
        return;

    main_loadview(to, from);
    backend_play("select");

}

void view_unselect(struct view *view, unsigned int key, char *from)
{

    if (key != KEY_B)
        return;

    main_quitview(from);
    backend_play("unselect");

}

void view_addselection(struct view *view, struct widget *widget)
{

    list_add(&view->selection.list, &widget->selectionitem);

}

void view_renderselection(struct view *view, unsigned int ticks)
{

    if (view->selection.active)
    {

        struct widget *widget = view->selection.active;

        backend_paint_selection(widget->size.x, widget->size.y, widget->size.w, widget->size.h);

    }

}

void view_reset(struct view *view)
{

    if (view->selection.list.head)
        view->selection.active = view->selection.list.head->data;
    else
        view->selection.active = 0;

}

void view_register(struct view *view, struct widget *widget)
{

    list_add(&view->widgets, &widget->item);

}

void view_init(struct view *view, char *name, void (*load)(void), void (*step)(unsigned int ticks), void (*config)(char *key, void *value), void (*button)(unsigned int key))
{

    view->name = name;
    view->load = load;
    view->step = step;
    view->config = config;
    view->button = button;

    list_inititem(&view->item, view);
    widget_area_init(&view->main, "main", "", 0, 0, 8, 8);
    view_register(view, &view->main);

}

