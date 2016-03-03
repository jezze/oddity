#include <stdlib.h>
#include "view.h"

void view_quit(struct view *view)
{

    if (view->onquit)
        view->onquit();

}

void view_init(struct view *view, void (*show)(), void (*render)(), void (*keydown)(unsigned int key))
{

    view->show = show;
    view->render = render;
    view->keydown = keydown;

}

