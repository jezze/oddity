#include <stdlib.h>
#include "define.h"
#include "box.h"

void box_init(struct box *box, unsigned int x, unsigned int y, unsigned int w, unsigned int h)
{

    box->x = x + RENDER_PADDING;
    box->y = y + RENDER_PADDING;
    box->w = w - (2 * RENDER_PADDING);
    box->h = h - (2 * RENDER_PADDING);

}

