#include <stdlib.h>
#include "backend.h"
#include "box.h"
#include "text.h"
#include "list.h"
#include "widget.h"

void widget_area_bounds(struct widget_area *area, struct box *box)
{

    box->x = area->size.x;
    box->y = area->size.y;
    box->w = area->size.w;
    box->h = area->size.h;

}

void widget_area_place(struct widget_area *area, int x, int y, int w, int h)
{

    unsigned int ow = 12;
    unsigned int oh = 12;
    unsigned int col = (w - ow * 2) / 8;
    unsigned int row = (h - oh * 2) / 8;

    area->size.x = col * area->unit.x + ow;
    area->size.y = row * area->unit.y + oh;
    area->size.w = col * area->unit.w;
    area->size.h = row * area->unit.h;

}

void widget_area_placein(struct widget_area *area, struct box *box)
{

    widget_area_place(area, box->x, box->y, box->w, box->h);

}

void widget_area_render(struct widget_area *area)
{

    backend_rect(area->size.x, area->size.y, area->size.w, area->size.h);

}

void widget_area_init(struct widget_area *area, int x, int y, int w, int h)
{

    list_inititem(&area->item, area);

    area->unit.x = x;
    area->unit.y = y;
    area->unit.w = w;
    area->unit.h = h;

}

void widget_slider_bounds(struct widget_slider *slider, struct box *box)
{

    box->x = slider->size.x;
    box->y = slider->size.y;
    box->w = slider->size.w;
    box->h = slider->size.h;

}

void widget_slider_place(struct widget_slider *slider, int x, int y, int w, int h)
{

    slider->size.x = x;
    slider->size.y = y;
    slider->size.w = w;
    slider->size.h = h;

}

void widget_slider_placein(struct widget_slider *slider, struct box *box)
{

    widget_slider_place(slider, box->x, box->y, box->w, box->h);

}

void widget_slider_render(struct widget_slider *slider)
{

    struct box box;

    box.x = slider->size.x + 10;
    box.y = slider->size.y + 10;
    box.w = slider->size.w - 10 * 2;
    box.h = slider->size.h - 10 * 2;

    backend_slider(box.x, box.y, box.w, box.h);

}

void widget_slider_init(struct widget_slider *slider, int min, int max, int value)
{

    list_inititem(&slider->item, slider);

    slider->min = min;
    slider->max = max;
    slider->value = value;

}

void widget_text_bounds(struct widget_text *text, struct box *box)
{

    box->x = text->size.x;
    box->y = text->size.y;
    box->w = text->size.w;
    box->h = text->size.h;

}

void widget_text_place(struct widget_text *text, int x, int y, int w, int h)
{

    text->size.x = x;
    text->size.y = y;
    text->size.w = w;
    text->size.h = h;

}

void widget_text_placein(struct widget_text *text, struct box *box)
{

    widget_text_place(text, box->x, box->y, box->w, box->h);

}

void widget_text_render(struct widget_text *text)
{

    struct box box;

    box.x = text->size.x + 10;
    box.y = text->size.y + 6;
    box.w = text->size.w - 10 * 2;
    box.h = text->size.h - 6 * 2;

    text_render2(&box, text->color, text->align, text->data);

}

void widget_text_init(struct widget_text *text, unsigned int color, unsigned int align, char *data)
{

    list_inititem(&text->item, text);

    text->color = color;
    text->align = align;
    text->data = data;

}

