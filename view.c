#include <stdlib.h>
#include "view.h"

static struct view *currentview;

void view_set(struct view *view)
{

    if (currentview)
        currentview->ondestroy();

    currentview = view;

    if (currentview)
        currentview->oninit();

}

void view_render()
{

    currentview->onrender();

}

void view_key(unsigned int keysym)
{

    currentview->onkey(keysym);

}

void view_init(struct view *view, void (*oninit)(), void (*ondestroy)(), void (*onrender)(), void (*onkey)(unsigned int keysym))
{

    view->oninit = oninit;
    view->ondestroy = ondestroy;
    view->onrender = onrender;
    view->onkey = onkey;

}

