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

    currentview->destroy();

    currentview = view;

    currentview->init();

}

int main(int argc, char **argv)
{

    struct view *showappview = view_showappsetup(SCREEN_WIDTH, SCREEN_HEIGHT);
    struct view *browseview = view_browsesetup(SCREEN_WIDTH, SCREEN_HEIGHT, showappview);
    struct view *categoryview = view_categorysetup(SCREEN_WIDTH, SCREEN_HEIGHT, browseview);
    struct view *frontview = view_frontsetup(SCREEN_WIDTH, SCREEN_HEIGHT, categoryview);

    view_setparent(showappview, browseview);
    view_setparent(browseview, categoryview);
    view_setparent(categoryview, frontview);
    view_setparent(frontview, frontview);

    currentview = frontview;

    render_init();
    render_initfont();
    render_update(currentview);

    while (currentstate)
    {

        render_waitevent(currentview);
        render_update(currentview);

    }

    render_destroyfont();
    render_destroy();

    return 0;

}

