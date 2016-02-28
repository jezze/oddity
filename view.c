#include <stdlib.h>
#include "view.h"

static struct view *currentview;

void view_set(struct view *view)
{

    if (currentview)
        currentview->destroy();

    currentview = view;

    if (currentview)
        currentview->init();

}

void view_render()
{

    currentview->render();

}

void view_key(unsigned int keysym)
{

    currentview->key(keysym);

}

void view_init(struct view *view, void (*init)(), void (*destroy)(), void (*render)(), void (*key)(unsigned int keysym))
{

    view->init = init;
    view->destroy = destroy;
    view->render = render;
    view->key = key;

}

