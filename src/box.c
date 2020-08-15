#include <stdlib.h>
#include "define.h"
#include "box.h"

#define PADDING                         12

void box_setpartsize(struct box *box, unsigned int pw, unsigned int ph, unsigned int x, unsigned int y, unsigned int w, unsigned int h)
{

    box->x = x * pw + PADDING;
    box->y = y * ph + PADDING;
    box->w = w * pw - (2 * PADDING);
    box->h = h * ph - (2 * PADDING);

}

void box_init(struct box *box)
{

}

