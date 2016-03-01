#include <stdlib.h>
#include "view.h"

void view_setparent(struct view *view, struct view *parent)
{

    view->parent = parent;

}

void view_init(struct view *view, void (*show)(), void (*hide)(), void (*render)(), void (*keydown)(unsigned int key))
{

    view->show = show;
    view->hide = hide;
    view->render = render;
    view->keydown = keydown;

}

