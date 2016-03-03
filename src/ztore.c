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

    currentview->hide();

    quit = 1;

}

void ztore_flipview(struct view *view)
{

    currentview->hide();

    currentview = view;

    currentview->show();

}

int main(int argc, char **argv)
{

    struct view *app = view_app_setup(SCREEN_WIDTH, SCREEN_HEIGHT);
    struct view *applist = view_applist_setup(SCREEN_WIDTH, SCREEN_HEIGHT, app);
    struct view *repolist = view_repolist_setup(SCREEN_WIDTH, SCREEN_HEIGHT, applist);
    struct view *sync = view_sync_setup(SCREEN_WIDTH, SCREEN_HEIGHT);
    struct view *front = view_front_setup(SCREEN_WIDTH, SCREEN_HEIGHT, repolist, sync);

    view_setparent(app, applist);
    view_setparent(applist, repolist);
    view_setparent(repolist, front);
    view_setparent(sync, front);
    view_setparent(front, front);

    currentview = front;

    ztore_flipview(currentview);
    backend_init(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP);
    backend_loadbackground("back.png");
    backend_loadfont("habbo.ttf");
    backend_update(currentview);

    while (!quit)
    {

        backend_waitevent(currentview);
        backend_update(currentview);

    }

    backend_unloadbackground();
    backend_unloadfont();
    backend_destroy();

    return 0;

}

