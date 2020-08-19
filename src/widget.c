#include <stdlib.h>
#include "box.h"
#include "backend.h"
#include "text.h"
#include "list.h"
#include "widget.h"

static void widget_init(struct widget *widget, unsigned int type)
{

    widget->type = type;

    list_inititem(&widget->item, widget);

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

void widget_area_init(struct widget *widget, int x, int y, int w, int h)
{

    struct widget_area *area = &widget->payload.area;

    widget_init(widget, WIDGET_TYPE_AREA);

    area->unit.x = x;
    area->unit.y = y;
    area->unit.w = w;
    area->unit.h = h;
    area->selectable = 1;

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

void widget_slider_init(struct widget *widget, int min, int max, int value)
{

    struct widget_slider *slider = &widget->payload.slider;

    widget_init(widget, WIDGET_TYPE_SLIDER);

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

void widget_text_init(struct widget *widget, unsigned int color, unsigned int align, char *data)
{

    struct widget_text *text = &widget->payload.text;

    widget_init(widget, WIDGET_TYPE_TEXT);

    text->color = color;
    text->align = align;
    text->data = data;

}

