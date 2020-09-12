#include <stdlib.h>
#include <string.h>
#include "define.h"
#include "box.h"
#include "list.h"
#include "text.h"
#include "widget.h"
#include "view.h"
#include "backend.h"

#define DIVX 12
#define DIVY 8
#define BORX 1
#define BORY 2
#define PADX 8
#define PADY 6

static void widget_init(struct widget *widget, unsigned int type, char *id, char *in)
{

    widget->type = type;
    widget->id = id;
    widget->in = in;

    list_inititem(&widget->item, widget);

}

void widget_area_place(struct widget *widget, struct box *box)
{

    struct widget_area *area = &widget->payload.area;
    unsigned int x = box->w / DIVX - BORX;
    unsigned int y = box->h / DIVY - BORY;
    unsigned int mx = (box->w - (x * DIVX)) / 2;
    unsigned int my = (box->h - (y * DIVY)) / 2;

    widget->size.x = x * area->x + mx;
    widget->size.y = y * area->y + my;
    widget->size.w = x * area->w;
    widget->size.h = y * area->h;

}

void widget_area_render(struct widget *widget, unsigned int ticks)
{

}

void widget_area_set(struct widget *widget, char *key, char *value)
{

}

void widget_area_init(struct widget *widget, char *id, char *in, int x, int y, int w, int h)
{

    struct widget_area *area = &widget->payload.area;

    widget_init(widget, WIDGET_TYPE_AREA, id, in);

    area->x = x;
    area->y = y;
    area->w = w;
    area->h = h;

}

void widget_icon_place(struct widget *widget, struct box *box)
{

    widget->size.x = box->x;
    widget->size.y = box->y;
    widget->size.w = box->w;
    widget->size.h = box->h;

}

void widget_icon_render(struct widget *widget, unsigned int ticks)
{

    struct widget_icon *icon = &widget->payload.icon;
    struct box box;

    box.x = widget->size.x + PADX;
    box.y = widget->size.y + PADY;
    box.w = widget->size.w - PADX * 2;
    box.h = widget->size.h - PADY * 2;

    backend_paint_icon(box.x, box.y, box.w, box.h, icon->type);

}

void widget_icon_set(struct widget *widget, char *key, char *value)
{

}

void widget_icon_init(struct widget *widget, char *id, char *in, unsigned int type)
{

    struct widget_icon *icon = &widget->payload.icon;

    widget_init(widget, WIDGET_TYPE_ICON, id, in);

    icon->type = type;

}

void widget_slider_place(struct widget *widget, struct box *box)
{

    widget->size.x = box->x;
    widget->size.y = box->y;
    widget->size.w = box->w;
    widget->size.h = box->h;

}

void widget_slider_render(struct widget *widget, unsigned int ticks)
{

    struct widget_slider *slider = &widget->payload.slider;
    struct box box;

    box.x = widget->size.x + PADX;
    box.y = widget->size.y + PADY;
    box.w = widget->size.w - PADX * 2;
    box.h = widget->size.h - PADY * 2;

    if (slider->value >= 0)
    {

        unsigned int ws = box.w * ((float)slider->value / (float)100);

        backend_paint_slider(box.x, box.y, box.w, box.h, ws, ticks);

    }

    else
    {

        backend_paint_slider(box.x, box.y, box.w, box.h, -1, ticks);

    }

}

void widget_slider_set(struct widget *widget, char *key, char *value)
{

    struct widget_slider *slider = &widget->payload.slider;

    if (!strcmp(key, "value"))
    {

        slider->value = strtol(value, 0, 10);

    }

}

void widget_slider_init(struct widget *widget, char *id, char *in, int min, int max, int value)
{

    struct widget_slider *slider = &widget->payload.slider;

    widget_init(widget, WIDGET_TYPE_SLIDER, id, in);

    slider->min = min;
    slider->max = max;
    slider->value = value;

}

void widget_text_place(struct widget *widget, struct box *box)
{

    widget->size.x = box->x;
    widget->size.y = box->y;
    widget->size.w = box->w;
    widget->size.h = box->h;

}

void widget_text_render(struct widget *widget, unsigned int ticks)
{

    struct widget_text *text = &widget->payload.text;
    struct box box;

    box.x = widget->size.x + PADX;
    box.y = widget->size.y + PADY;
    box.w = widget->size.w - PADX * 2;
    box.h = widget->size.h - PADY * 2;

    if (text->data)
        text_render(&box, text->type, text->align, text->data);

}

void widget_text_set(struct widget *widget, char *key, char *value)
{

    struct widget_text *text = &widget->payload.text;

    if (!strcmp(key, "type"))
    {

        if (!strcmp(value, "normal"))
            text->type = TEXT_TYPE_NORMAL;
        else if (!strcmp(value, "title"))
            text->type = TEXT_TYPE_TITLE;
        else if (!strcmp(value, "select"))
            text->type = TEXT_TYPE_SELECT;
        else if (!strcmp(value, "disable"))
            text->type = TEXT_TYPE_DISABLE;

    }

    if (!strcmp(key, "align"))
    {

        if (!strcmp(value, "left"))
            text->align = TEXT_ALIGN_LEFT;
        else if (!strcmp(value, "right"))
            text->align = TEXT_ALIGN_RIGHT;
        else if (!strcmp(value, "center"))
            text->align = TEXT_ALIGN_CENTER;

    }

    if (!strcmp(key, "data"))
    {

        text->data = value;

    }

}

void widget_text_init(struct widget *widget, char *id, char *in, unsigned int type, unsigned int align, char *data)
{

    struct widget_text *text = &widget->payload.text;

    widget_init(widget, WIDGET_TYPE_TEXT, id, in);

    text->type = type;
    text->align = align;
    text->data = data;

}

void widget_toggle_place(struct widget *widget, struct box *box)
{

    widget->size.x = box->x;
    widget->size.y = box->y;
    widget->size.w = box->w;
    widget->size.h = box->h;

}

void widget_toggle_render(struct widget *widget, unsigned int ticks)
{

    struct widget_toggle *toggle = &widget->payload.toggle;
    struct box box;

    box.x = widget->size.x + PADX;
    box.y = widget->size.y + PADY;
    box.w = widget->size.w - PADX * 2;
    box.h = widget->size.h - PADY * 2;

    switch (toggle->state)
    {

    case TOGGLE_OFF:
        text_render(&box, TEXT_TYPE_NORMAL, TEXT_ALIGN_RIGHT, "Off");

        break;
    case TOGGLE_ON:
        text_render(&box, TEXT_TYPE_NORMAL, TEXT_ALIGN_RIGHT, "On");

        break;

    case TOGGLE_OFF_DISABLED:
        text_render(&box, TEXT_TYPE_DISABLE, TEXT_ALIGN_RIGHT, "Off");

        break;

    case TOGGLE_ON_DISABLED:
        text_render(&box, TEXT_TYPE_DISABLE, TEXT_ALIGN_RIGHT, "On");

        break;

    }

}

void widget_toggle_set(struct widget *widget, char *key, char *value)
{

    struct widget_toggle *toggle = &widget->payload.toggle;

    if (!strcmp(key, "state"))
    {

        if (!strcmp(value, "off"))
            toggle->state = TOGGLE_OFF;
        else if (!strcmp(value, "on"))
            toggle->state = TOGGLE_ON;
        else if (!strcmp(value, "off_disabled"))
            toggle->state = TOGGLE_OFF_DISABLED;
        else if (!strcmp(value, "on_disabled"))
            toggle->state = TOGGLE_ON_DISABLED;

    }

}

void widget_toggle_init(struct widget *widget, char *id, char *in, unsigned int state)
{

    struct widget_toggle *toggle = &widget->payload.toggle;

    widget_init(widget, WIDGET_TYPE_TOGGLE, id, in);

    toggle->state = state;

}

