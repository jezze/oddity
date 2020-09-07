#include <stdlib.h>
#include <string.h>
#include "define.h"
#include "box.h"
#include "list.h"
#include "widget.h"
#include "view.h"
#include "backend.h"
#include "main.h"

static struct widget widgets[512];
static unsigned int nwidgets;

static void placewidget(struct widget *widget, struct box *size)
{

    switch (widget->type)
    {

    case WIDGET_TYPE_AREA:
        widget_area_place(widget, size);

        break;

    case WIDGET_TYPE_ICON:
        widget_icon_place(widget, size);

        break;

    case WIDGET_TYPE_SLIDER:
        widget_slider_place(widget, size);

        break;

    case WIDGET_TYPE_TEXT:
        widget_text_place(widget, size);

        break;

    case WIDGET_TYPE_TOGGLE:
        widget_toggle_place(widget, size);

        break;

    }

}

static void placewidgets(struct view *view, struct widget *parent)
{

    struct list_item *current;

    for (current = view->widgets.head; current; current = current->next)
    {

        struct widget *widget = current->data;

        if (strlen(parent->id) && !strcmp(widget->in, parent->id))
        {

            placewidget(widget, &parent->size);
            placewidgets(view, widget);

        }

    }

}

static void renderwidget(struct widget *widget, unsigned int ticks)
{

    switch (widget->type)
    {

    case WIDGET_TYPE_AREA:
        widget_area_render(widget, ticks);

        break;

    case WIDGET_TYPE_ICON:
        widget_icon_render(widget, ticks);

        break;

    case WIDGET_TYPE_SLIDER:
        widget_slider_render(widget, ticks);

        break;

    case WIDGET_TYPE_TEXT:
        widget_text_render(widget, ticks);

        break;

    case WIDGET_TYPE_TOGGLE:
        widget_toggle_render(widget, ticks);

        break;

    }

}

static void renderwidgets(struct view *view, struct widget *parent, unsigned int ticks)
{

    struct list_item *current;

    for (current = view->widgets.head; current; current = current->next)
    {

        struct widget *widget = current->data;

        if (strlen(parent->id) && !strcmp(widget->in, parent->id))
        {

            renderwidget(widget, ticks);
            renderwidgets(view, widget, ticks);

        }

    }

}

struct widget *view_findwidget(struct view *view, char *id)
{

    struct list_item *current;

    for (current = view->widgets.head; current; current = current->next)
    {

        struct widget *widget = current->data;

        if (!strcmp(widget->id, id))
            return widget;

    }

    return 0;

}

struct widget *view_createwidget(struct view *view)
{

    struct widget *widget = &widgets[nwidgets++];

    list_add(&view->widgets, &widget->item);

    return widget;

}

void view_place(struct view *view)
{

    placewidgets(view, &view->main);

}

void view_render(struct view *view, unsigned int ticks)
{

    if (view->selection.active)
    {

        struct widget *widget = view->selection.active;

        backend_paint_selection(widget->size.x, widget->size.y, widget->size.w, widget->size.h);

    }

    renderwidgets(view, &view->main, ticks);

}

unsigned int view_isactive(struct view *view, char *id)
{

    if (!view->selection.active)
        return 0;

    return !strcmp(view->selection.active->id, id);

}

void view_moveselection(struct view *view, unsigned int key)
{

    struct list_item *current;
    struct widget *best = 0;
    int bestdx = 5000;
    int bestdy = 5000;
    int mx;
    int my;

    if (!(key == KEY_LEFT || key == KEY_RIGHT || key == KEY_UP || key == KEY_DOWN))
        return;

    if (!view->selection.active)
        return;

    mx = view->selection.active->size.x + view->selection.active->size.w / 2;
    my = view->selection.active->size.y + view->selection.active->size.h / 2;

    for (current = view->selection.list.head; current; current = current->next)
    {

        struct widget *widget = current->data;
        int dx = abs(widget->size.x - view->selection.active->size.x);
        int dy = abs(widget->size.y - view->selection.active->size.y);

        switch (key)
        {

        case KEY_LEFT:
            if (widget->size.x + widget->size.w >= mx)
                continue;

            if (dy <= bestdy && dx < bestdx)
            {

                best = widget;
                bestdx = dx;
                bestdy = dy;

            }

            break;

        case KEY_RIGHT:
            if (widget->size.x < mx)
                continue;

            if (dy <= bestdy && dx < bestdx)
            {

                best = widget;
                bestdx = dx;
                bestdy = dy;

            }

            break;

        case KEY_UP:
            if (widget->size.y + widget->size.h >= my)
                continue;

            if (dx <= bestdx && dy < bestdy)
            {

                best = widget;
                bestdx = dx;
                bestdy = dy;

            }

            break;

        case KEY_DOWN:
            if (widget->size.y < my)
                continue;

            if (dx <= bestdx && dy < bestdy)
            {

                best = widget;
                bestdx = dx;
                bestdy = dy;

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

void view_addselection(struct view *view, char *id)
{

    struct widget *widget = view_findwidget(view, id);

    if (widget)
        list_add(&view->selection.list, &widget->selectionitem);

}

void view_reset(struct view *view)
{

    if (view->selection.list.head)
        view->selection.active = view->selection.list.head->data;
    else
        view->selection.active = 0;

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
    list_add(&view->widgets, &view->main.item);

}

