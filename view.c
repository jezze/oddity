#include <stdlib.h>
#include "view.h"

void view_init(struct view *view, void (*init)(), void (*destroy)(), void (*render)(), void (*key)(unsigned int keysym))
{

    view->init = init;
    view->destroy = destroy;
    view->render = render;
    view->key = key;

}

