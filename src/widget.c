#include <stdlib.h>
#include "backend.h"
#include "box.h"
#include "text.h"
#include "list.h"
#include "widget.h"

void widget_area_place(struct widget_area *area, int x, int y, int w, int h)
{

    unsigned int ow = 4;
    unsigned int oh = 6;
    unsigned int col = (w - ow * 2) / 6;
    unsigned int row = (h - oh * 2) / 6;

    area->size.x = col * area->unit.x + ow;
    area->size.y = row * area->unit.y + oh;
    area->size.w = col * area->unit.w;
    area->size.h = row * area->unit.h;

}

void widget_area_render(struct widget_area *area)
{

    struct box box;

    box.x = area->size.x + 4;
    box.y = area->size.y + 4;
    box.w = area->size.w - 8;
    box.h = area->size.h - 8;

    backend_rect(box.x, box.y, box.w, box.h);

}

void widget_area_init(struct widget_area *area, int x, int y, int w, int h)
{

    list_inititem(&area->item, area);

    area->unit.x = x;
    area->unit.y = y;
    area->unit.w = w;
    area->unit.h = h;

}

void widget_text_place(struct widget_text *text, int x, int y, int w, int h)
{

    text->size.x = x;
    text->size.y = y;
    text->size.w = w;
    text->size.h = h;

}

void widget_text_render(struct widget_text *text)
{

    text_render2(&text->size, text->color, text->align, text->data);

}

void widget_text_init(struct widget_text *text, unsigned int color, unsigned int align, char *data)
{

    list_inititem(&text->item, text);

    text->color = color;
    text->align = align;
    text->data = data;

}

