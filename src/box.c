#include <stdlib.h>
#include "box.h"

void box_init(struct box *box, unsigned int x, unsigned int y, unsigned int w, unsigned int h)
{

    box->x = x;
    box->y = y;
    box->w = w;
    box->h = h;

}

