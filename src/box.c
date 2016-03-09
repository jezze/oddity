#include <stdlib.h>
#include "define.h"
#include "box.h"

void box_setpartsize(struct box *box, unsigned int pw, unsigned int ph, unsigned int x, unsigned int y, unsigned int w, unsigned int h)
{

    box->x = x * pw + RENDER_PADDING;
    box->y = y * ph + RENDER_PADDING;
    box->w = w * pw - (2 * RENDER_PADDING);
    box->h = h * ph - (2 * RENDER_PADDING);

}

void box_init(struct box *box)
{

}

