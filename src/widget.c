#include <stdlib.h>
#include "box.h"
#include "backend.h"
#include "text.h"
#include "list.h"
#include "widget.h"

void widget_area_place(struct widget_area *area, struct box *box)
{

    unsigned int ow = 12;
    unsigned int oh = 12;
    unsigned int col = (box->w - ow * 2) / 8;
    unsigned int row = (box->h - oh * 2) / 8;

    area->size.x = col * area->unit.x + ow;
    area->size.y = row * area->unit.y + oh;
    area->size.w = col * area->unit.w;
    area->size.h = row * area->unit.h;

}

void widget_area_render(struct widget_area *area, unsigned int ticks)
{

}

void widget_area_init(struct widget_area *area, int x, int y, int w, int h)
{

    list_inititem(&area->item, area);

    area->unit.x = x;
    area->unit.y = y;
    area->unit.w = w;
    area->unit.h = h;
    area->selectable = 1;

}

void widget_slider_place(struct widget_slider *slider, struct box *box)
{

    slider->size.x = box->x;
    slider->size.y = box->y;
    slider->size.w = box->w;
    slider->size.h = box->h;

}

void widget_slider_render(struct widget_slider *slider, unsigned int ticks)
{

    struct box box;

    box.x = slider->size.x + 10;
    box.y = slider->size.y + 8;
    box.w = slider->size.w - 10 * 2;
    box.h = slider->size.h - 8 * 2;

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

void widget_slider_init(struct widget_slider *slider, int min, int max, int value)
{

    list_inititem(&slider->item, slider);

    slider->min = min;
    slider->max = max;
    slider->value = value;

}

void widget_text_place(struct widget_text *text, struct box *box)
{

    text->size.x = box->x;
    text->size.y = box->y;
    text->size.w = box->w;
    text->size.h = box->h;

}

void widget_text_render(struct widget_text *text, unsigned int ticks)
{

    struct box box;

    box.x = text->size.x + 10;
    box.y = text->size.y + 6;
    box.w = text->size.w - 10 * 2;
    box.h = text->size.h - 6 * 2;

    text_render(&box, text->color, text->align, text->data);

}

void widget_text_init(struct widget_text *text, unsigned int color, unsigned int align, char *data)
{

    list_inititem(&text->item, text);

    text->color = color;
    text->align = align;
    text->data = data;

}

