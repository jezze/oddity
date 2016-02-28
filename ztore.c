#include <stdlib.h>
#include <stdarg.h>
#include "ztore.h"
#include "view.h"
#include "event.h"
#include "box.h"
#include "text.h"
#include "menu.h"
#include "render.h"

static unsigned int currentstate = 1;

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
    view_render();
    render_flip();

    while (currentstate)
    {

        render_waitevent();
        view_render();
        render_flip();

    }

    render_destroyfont();
    render_destroy();

    return 0;

}

