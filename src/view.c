#include <stdlib.h>
#include "view.h"

void view_quit(struct view *view)
{

    if (view->onquit)
        view->onquit();

}

void view_init(struct view *view, void (*load)(), void (*render)(), void (*keydown)(unsigned int key))
{

    view->load = load;
    view->render = render;
    view->keydown = keydown;

}

