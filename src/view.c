#include <stdlib.h>
#include "view.h"

void view_load(struct view *view, struct view *parent)
{

    view->parent = parent;

    if (view->load)
        view->load();

}

void view_quit(struct view *view)
{

    if (view->parent)
        view_load(view->parent, view->parent->parent);

}

void view_init(struct view *view, void (*load)(void))
{

    view->load = load;

}

