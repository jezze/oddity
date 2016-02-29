#include <stdlib.h>
#include <stdarg.h>
#include "view.h"
#include "box.h"
#include "text.h"
#include "menu.h"
#include "backend.h"
#include "ztore.h"

#define SCREEN_WIDTH                    320
#define SCREEN_HEIGHT                   240
#define SCREEN_BPP                      32

static struct view *currentview;
static unsigned int quit;

void ztore_quit()
{

    currentview->destroy();

    quit = 1;

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

    backend_init(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP);
    backend_update(currentview);

    while (!quit)
    {

        backend_waitevent(currentview);
        backend_update(currentview);

    }

    backend_destroy();

    return 0;

}

