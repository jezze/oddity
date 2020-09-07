#include <stdlib.h>
#include "define.h"
#include "box.h"
#include "list.h"
#include "text.h"
#include "widget.h"
#include "view.h"
#include "backend.h"

static void widget_init(struct widget *widget, unsigned int type, char *id, char *in)
{

    widget->type = type;
    widget->id = id;
    widget->in = in;

    list_inititem(&widget->item, widget);
    list_inititem(&widget->selectionitem, widget);

}

void widget_area_place(struct widget *widget, struct box *box)
{

    struct widget_area *area = &widget->payload.area;
    unsigned int ow = 12;
    unsigned int oh = 12;
    unsigned int col = (box->w - ow * 2) / 8;
    unsigned int row = (box->h - oh * 2) / 8;

    widget->size.x = col * area->unit.x + ow;
    widget->size.y = row * area->unit.y + oh;
    widget->size.w = col * area->unit.w;
    widget->size.h = row * area->unit.h;

}

void widget_area_render(struct widget *widget, unsigned int ticks)
{

}

void widget_area_init(struct widget *widget, char *id, char *in, int x, int y, int w, int h)
{

    struct widget_area *area = &widget->payload.area;

    widget_init(widget, WIDGET_TYPE_AREA, id, in);

    area->unit.x = x;
    area->unit.y = y;
    area->unit.w = w;
    area->unit.h = h;
    area->selectable = 1;

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

    box.x = widget->size.x + 10;
    box.y = widget->size.y + 6;
    box.w = widget->size.w - 10 * 2;
    box.h = widget->size.h - 6 * 2;

    backend_paint_icon(box.x, box.y, box.w, box.h, icon->type);

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

    box.x = widget->size.x + 10;
    box.y = widget->size.y + 8;
    box.w = widget->size.w - 10 * 2;
    box.h = widget->size.h - 8 * 2;

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

    box.x = widget->size.x + 10;
    box.y = widget->size.y + 6;
    box.w = widget->size.w - 10 * 2;
    box.h = widget->size.h - 6 * 2;

    text_render(&box, text->color, text->align, text->data);

}

void widget_text_init(struct widget *widget, char *id, char *in, unsigned int color, unsigned int align, char *data)
{

    struct widget_text *text = &widget->payload.text;

    widget_init(widget, WIDGET_TYPE_TEXT, id, in);

    text->color = color;
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

    box.x = widget->size.x + 10;
    box.y = widget->size.y + 6;
    box.w = widget->size.w - 10 * 2;
    box.h = widget->size.h - 6 * 2;

    switch (toggle->state)
    {

    case TOGGLE_OFF:
        text_render(&box, TEXT_COLOR_NORMAL, TEXT_ALIGN_RIGHT, "Off");

        break;
    case TOGGLE_ON:
        text_render(&box, TEXT_COLOR_NORMAL, TEXT_ALIGN_RIGHT, "On");

        break;

    case TOGGLE_OFF_DISABLED:
        text_render(&box, TEXT_COLOR_DISABLE, TEXT_ALIGN_RIGHT, "Off");

        break;

    case TOGGLE_ON_DISABLED:
        text_render(&box, TEXT_COLOR_DISABLE, TEXT_ALIGN_RIGHT, "On");

        break;

    }

}

void widget_toggle_init(struct widget *widget, char *id, char *in, unsigned int state)
{

    struct widget_toggle *toggle = &widget->payload.toggle;

    widget_init(widget, WIDGET_TYPE_TOGGLE, id, in);

    toggle->state = state;

}

