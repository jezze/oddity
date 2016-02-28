#include <stdlib.h>
#include <stdarg.h>
#include "ztore.h"
#include "event.h"
#include "text.h"
#include "menu.h"
#include "render.h"

static struct view *currentview;
static unsigned int currentstate = 1;

void view_set(struct view *view)
{

    if (currentview)
        currentview->ondestroy();

    currentview = view;

    if (currentview)
        currentview->oninit();

}

void view_init(struct view *view, void (*oninit)(), void (*ondestroy)(), void (*onrender)(), void (*onkey)(unsigned int keysym))
{

    view->oninit = oninit;
    view->ondestroy = ondestroy;
    view->onrender = onrender;
    view->onkey = onkey;

}

static void onevent(unsigned int type, void *data)
{

    switch (type)
    {

    case EVENT_TYPE_QUIT:
        currentstate = 0;

        break;

    }

}

int main(int argc, char **argv)
{

    event_register(onevent);
    view_frontsetup(SCREEN_WIDTH, SCREEN_HEIGHT);
    view_appssetup(SCREEN_WIDTH, SCREEN_HEIGHT);
    view_browsesetup(SCREEN_WIDTH, SCREEN_HEIGHT);
    view_showappsetup(SCREEN_WIDTH, SCREEN_HEIGHT);
    render_init();
    render_initfont();
    currentview->onrender();
    render_flip();

    while (currentstate)
    {

        render_waitevent(currentview);
        currentview->onrender();
        render_flip();

    }

    render_destroyfont();
    render_destroy();

    return 0;

}

