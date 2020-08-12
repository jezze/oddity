#include <stdlib.h>
#include "backend.h"
#include "box.h"
#include "list.h"
#include "area.h"

void area_place(struct area *area, int w, int h)
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

void area_render(struct area *area)
{

    struct box box;

    box.x = area->size.x + 4;
    box.y = area->size.y + 4;
    box.w = area->size.w - 8;
    box.h = area->size.h - 8;

    backend_rect(box.x, box.y, box.w, box.h);

}

void area_init(struct area *area, int x, int y, int w, int h)
{

    list_inititem(&area->item, area);

    area->unit.x = x;
    area->unit.y = y;
    area->unit.w = w;
    area->unit.h = h;

}

