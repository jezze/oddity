#include <stdlib.h>
#include <stdarg.h>
#include "ztore.h"
#include "text.h"
#include "menu.h"
#include "render.h"

struct view views[64];
struct view *currentview;
unsigned int currentstate = 1;

void ztore_changestate(unsigned int state)
{

    currentstate = state;

}

void view_set(unsigned int index, unsigned int from, unsigned int value)
{

    if (currentview && currentview->destroy)
        currentview->destroy();

    currentview = &views[index];

    if (currentview && currentview->init)
        currentview->init(from, value);

}

void view_handleevent(unsigned int index, unsigned int id)
{

    struct view *view = &views[index];

    view->handleevent(id);

}

void view_register(unsigned int index, void (*init)(), void (*destroy)(), void (*render)(), void (*handlekey)(unsigned int id), void (*handleevent)(unsigned int id))
{

    struct view *view = &views[index];

    view->init = init;
    view->destroy = destroy;
    view->render = render;
    view->handlekey = handlekey;
    view->handleevent = handleevent;

}

int main(int argc, char **argv)
{

    view_frontsetup();
    view_appssetup();
    view_browsesetup();
    view_showappsetup();
    view_set(0, 0, 0);
    render_init();
    render_initfont();
    currentview->render();
    render_flip();

    while (currentstate)
    {

        render_waitevent(currentview);
        currentview->render();
        render_flip();

    }

    render_destroyfont();
    render_destroy();

    return 0;

}

