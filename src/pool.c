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

void pool_area_create(struct view *view, char *id, char *in, int x, int y, int w, int h)
{

    struct widget *widget = createwidget(view);

    widget_area_init(widget, id, in, x, y, w, h);

}

void pool_icon_create(struct view *view, char *id, char *in, unsigned int type)
{

    struct widget *widget = createwidget(view);

    widget_icon_init(widget, id, in, type);

}

void pool_option_create(struct view *view, char *id, char *in, char *name, char *value)
{

    struct widget *widget = createwidget(view);

    widget_option_init(widget, id, in, name, value);

}

void pool_select_create(struct view *view, char *id, char *in)
{

    struct widget *widget = createwidget(view);

    widget_select_init(widget, id, in);

}

void pool_slider_create(struct view *view, char *id, char *in, int min, int max, int value)
{

    struct widget *widget = createwidget(view);

    widget_slider_init(widget, id, in, min, max, value);

}

void pool_text_create(struct view *view, char *id, char *in, unsigned int type, unsigned int align, char *data)
{

    struct widget *widget = createwidget(view);

    widget_text_init(widget, id, in, type, align, data);

}

void pool_toggle_create(struct view *view, char *id, char *in, unsigned int state)
{

    struct widget *widget = createwidget(view);

    widget_toggle_init(widget, id, in, state);

}

