#include <stdlib.h>
#include <string.h>
#include "config.h"
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

    case WIDGET_TYPE_ICON:
        widget_icon_place(widget, size);

        break;

    case WIDGET_TYPE_OPTION:
        widget_option_place(widget, size);

        break;

    case WIDGET_TYPE_SELECT:
        widget_select_place(widget, size);

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

    case WIDGET_TYPE_OPTION:
        widget_option_render(widget, ticks);

        break;

    case WIDGET_TYPE_SELECT:
        widget_select_render(widget, ticks);

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

            if (widget->hidden)
                continue;

            renderwidget(widget, ticks);
            renderwidgets(view, widget, ticks);

        }

    }

}

static void setwidget(struct widget *widget, char *key, char *value)
{

    if (!strcmp(key, "hidden"))
    {

        if (!strcmp(value, "true"))
            widget->hidden = 1;
        else if (!strcmp(value, "false"))
            widget->hidden = 0;

    }

    if (!strcmp(key, "selectable"))
    {

        if (!strcmp(value, "true"))
            widget->selectable = 1;
        else if (!strcmp(value, "false"))
            widget->selectable = 0;

    }

    switch (widget->type)
    {

    case WIDGET_TYPE_AREA:
        widget_area_set(widget, key, value);

        break;

    case WIDGET_TYPE_ICON:
        widget_icon_set(widget, key, value);

        break;

    case WIDGET_TYPE_OPTION:
        widget_option_set(widget, key, value);

        break;

    case WIDGET_TYPE_SELECT:
        widget_select_set(widget, key, value);

        break;

    case WIDGET_TYPE_SLIDER:
        widget_slider_set(widget, key, value);

        break;

    case WIDGET_TYPE_TEXT:
        widget_text_set(widget, key, value);

        break;

    case WIDGET_TYPE_TOGGLE:
        widget_toggle_set(widget, key, value);

        break;

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

void view_place(struct view *view)
{

    placewidgets(view, &view->main);

}

void view_render(struct view *view, unsigned int ticks)
{

    if (view->selected)
    {

        struct widget *widget = view->selected;

        backend_paint_selection(widget->size.x, widget->size.y, widget->size.w, widget->size.h);

    }

    renderwidgets(view, &view->main, ticks);

}

unsigned int view_isactive(struct view *view, char *id)
{

    if (!view->selected)
        return 0;

    return !strcmp(view->selected->id, id);

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

    if (!view->selected)
        return;

    mx = view->selected->size.x + view->selected->size.w / 2;
    my = view->selected->size.y + view->selected->size.h / 2;

    for (current = view->widgets.head; current; current = current->next)
    {

        struct widget *widget = current->data;
        int dx = abs(widget->size.x - view->selected->size.x);
        int dy = abs(widget->size.y - view->selected->size.y);

        if (!widget->selectable)
            continue;

        switch (key)
        {

        case KEY_LEFT:
            if (widget->size.x + widget->size.w >= mx)
                continue;

            if (dy <= bestdy && dx <= bestdx)
            {

                best = widget;
                bestdx = dx;
                bestdy = dy;

            }

            break;

        case KEY_RIGHT:
            if (widget->size.x < mx)
                continue;

            if (dy <= bestdy && dx <= bestdx)
            {

                best = widget;
                bestdx = dx;
                bestdy = dy;

            }

            break;

        case KEY_UP:
            if (widget->size.y + widget->size.h >= my)
                continue;

            if (dx <= bestdx && dy <= bestdy)
            {

                best = widget;
                bestdx = dx;
                bestdy = dy;

            }

            break;

        case KEY_DOWN:
            if (widget->size.y < my)
                continue;

            if (dx <= bestdx && dy <= bestdy)
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

        view->selected = best;

        backend_play("click");

    }

}

void view_select(struct view *view, unsigned int key, char *match, char *to)
{

    if (key != KEY_A)
        return;

    if (!view->selected)
        return;

    if (!strlen(view->selected->id))
        return;

    if (strcmp(view->selected->id, match))
        return;

    main_initview(to, view->name);
    backend_play("select");

}

void view_unselect(struct view *view, unsigned int key)
{

    if (key != KEY_B)
        return;

    main_destroyview(view->name);
    backend_play("unselect");

}

void view_goprev(struct view *view, unsigned int key, char *id)
{

    struct widget *widget;
    struct widget *child = 0;

    if (key != KEY_LEFT)
        return;

    widget = view_findwidget(view, id);

    if (!widget)
        return;

    while ((child = view_widget_nextchild(view, child, widget)))
    {

        if (widget->payload.select.value == child->payload.option.value)
            break;

    }

    if (child)
    {

        child = view_widget_prevchild(view, child, widget);

        if (child)
            widget->payload.select.value = child->payload.option.value;

    }

}

void view_gonext(struct view *view, unsigned int key, char *id)
{

    struct widget *widget;
    struct widget *child = 0;

    if (key != KEY_RIGHT)
        return;

    widget = view_findwidget(view, id);

    if (!widget)
        return;

    while ((child = view_widget_nextchild(view, child, widget)))
    {

        if (widget->payload.select.value == child->payload.option.value)
            break;

    }

    if (child)
    {

        child = view_widget_nextchild(view, child, widget);

        if (child)
            widget->payload.select.value = child->payload.option.value;

    }

}

void view_reset(struct view *view)
{

    struct list_item *current;

    view->selected = 0;

    for (current = view->widgets.head; current; current = current->next)
    {

        struct widget *widget = current->data;

        if (widget->selectable)
        {

            view->selected = widget;

            return;

        }

    }

}

void view_setattr(struct view *view, char *id, char *key, char *value)
{

    struct widget *widget = view_findwidget(view, id);

    if (!widget)
        return;

    setwidget(widget, key, value);

}

static struct widget *prevwidget(struct list *list, struct widget *widget)
{

    struct list_item *current = (widget) ? widget->item.prev : list->tail;

    return (current) ? current->data : 0;

}

static struct widget *nextwidget(struct list *list, struct widget *widget)
{

    struct list_item *current = (widget) ? widget->item.next : list->head;

    return (current) ? current->data : 0;

}

struct widget *view_widget_prev(struct view *view, struct widget *widget)
{

    return prevwidget(&view->widgets, widget);

}

struct widget *view_widget_next(struct view *view, struct widget *widget)
{

    return nextwidget(&view->widgets, widget);

}

struct widget *view_widget_prevchild(struct view *view, struct widget *widget, struct widget *parent)
{

    while ((widget = view_widget_prev(view, widget)))
    {

        if (view_findwidget(view, widget->in) == parent)
            return widget;

    }

    return 0;

}

struct widget *view_widget_nextchild(struct view *view, struct widget *widget, struct widget *parent)
{

    while ((widget = view_widget_next(view, widget)))
    {

        if (view_findwidget(view, widget->in) == parent)
            return widget;

    }

    return 0;

}

void view_precheck(struct view *view)
{

    struct widget *child = 0;

    while ((child = view_widget_next(view, child)))
    {

        if (child->type == WIDGET_TYPE_OPTION)
        {

            struct widget *parent = view_findwidget(view, child->in);

            if (!parent)
                continue;

            if (parent->type != WIDGET_TYPE_SELECT)
                continue;

            if (strcmp(child->payload.option.value, parent->payload.select.value))
                child->hidden = 1;
            else
                child->hidden = 0;

        }

    }

}

void view_init(struct view *view, char *name, void (*onload)(unsigned int type), void (*onstep)(unsigned int ticks), void (*onconfig)(char *key, void *value), void (*onbutton)(unsigned int key))
{

    view->name = name;
    view->onload = onload;
    view->onstep = onstep;
    view->onconfig = onconfig;
    view->onbutton = onbutton;

    list_inititem(&view->item, view);
    widget_area_init(&view->main, "main", "", 0, 0, 8, 8);
    list_add(&view->widgets, &view->main.item);

}

