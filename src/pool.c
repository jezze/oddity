#include "config.h"
#include "box.h"
#include "list.h"
#include "widget.h"
#include "view.h"

static struct widget widgets[MAXWIDGETS];
static unsigned int nwidgets;

static struct widget *createwidget(struct view *view)
{

    struct widget *widget = &widgets[nwidgets++];

    list_add(&view->widgets, &widget->item);

    return widget;

}

void pool_create_area(struct view *view, char *id, char *in, int x, int y, int w, int h)
{

    struct widget *widget = createwidget(view);

    widget_area_init(widget, id, in, x, y, w, h);

}

void pool_create_icon(struct view *view, char *id, char *in, unsigned int type)
{

    struct widget *widget = createwidget(view);

    widget_icon_init(widget, id, in, type);

}

void pool_create_option(struct view *view, char *id, char *in, char *name, char *value)
{

    struct widget *widget = createwidget(view);

    widget_option_init(widget, id, in, name, value);

}

void pool_create_select(struct view *view, char *id, char *in)
{

    struct widget *widget = createwidget(view);

    widget_select_init(widget, id, in);

}

void pool_create_slider(struct view *view, char *id, char *in, int min, int max, int value)
{

    struct widget *widget = createwidget(view);

    widget_slider_init(widget, id, in, min, max, value);

}

void pool_create_text(struct view *view, char *id, char *in, unsigned int type, unsigned int align, char *data)
{

    struct widget *widget = createwidget(view);

    widget_text_init(widget, id, in, type, align, data);

}

void pool_create_toggle(struct view *view, char *id, char *in, unsigned int state)
{

    struct widget *widget = createwidget(view);

    widget_toggle_init(widget, id, in, state);

}

