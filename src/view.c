#include <stdlib.h>
#include "backend.h"
#include "view.h"

static struct view *currentview;
static unsigned int quit;

void view_quitloop(void)
{

    quit = 1;

}

void view_redraw(void)
{

    backend_pollevent(view_quitloop, currentview->keydown);
    backend_render(currentview->render);

}

void view_load(struct view *view)
{

    if (view->preload)
        view->preload();

    currentview = view;

    currentview->load();

}

void view_loop(void)
{

    while (!quit)
    {

        backend_render(currentview->render);
        backend_waitevent(view_quitloop, currentview->keydown);

    }

}

void view_setmode(struct view *view, void (*render)(void), void (*keydown)(unsigned int key))
{

    view->render = render;
    view->keydown = keydown;

}

void view_quit(struct view *view)
{

    if (view->onquit)
        view->onquit();

}

void view_init(struct view *view, void (*load)(void), void (*render)(void), void (*keydown)(unsigned int key))
{

    view->load = load;
    view->render = render;
    view->keydown = keydown;

}

