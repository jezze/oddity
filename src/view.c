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

void view_init(struct view *view, char *name, void (*onload)(unsigned int type), void (*onstep)(unsigned int ticks), void (*onconfig)(char *key, void *value), void (*onbutton)(unsigned int button))
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

