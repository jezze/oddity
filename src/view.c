#include <stdlib.h>
#include "view.h"

void view_setparent(struct view *view, struct view *parent)
{

    view->parent = parent;

}

void view_init(struct view *view, void (*init)(), void (*destroy)(), void (*render)(), void (*keydown)(unsigned int key))
{

    view->init = init;
    view->destroy = destroy;
    view->render = render;
    view->keydown = keydown;

}

