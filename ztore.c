#include <stdlib.h>
#include <stdarg.h>
#include "view.h"
#include "box.h"
#include "text.h"
#include "menu.h"
#include "render.h"
#include "ztore.h"

static struct view *currentview;
static unsigned int currentstate = 1;

void ztore_quit()
{

    currentstate = 0;

}

void ztore_setview(struct view *view)
{

    if (currentview)
        currentview->destroy();

    currentview = view;

    if (currentview)
        currentview->init();

}

int main(int argc, char **argv)
{

    struct view *browseview;
    struct view *showappview;
    struct view *appview;
    struct view *frontview;

    showappview = view_showappsetup(SCREEN_WIDTH, SCREEN_HEIGHT);
    browseview = view_browsesetup(SCREEN_WIDTH, SCREEN_HEIGHT, showappview);
    appview = view_appssetup(SCREEN_WIDTH, SCREEN_HEIGHT);
    frontview = view_frontsetup(SCREEN_WIDTH, SCREEN_HEIGHT, appview, browseview);

    showappview->parent = browseview;
    browseview->parent = frontview;
    appview->parent = frontview;

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

