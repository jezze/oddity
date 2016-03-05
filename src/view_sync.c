#include <stdlib.h>
#include "define.h"
#include "box.h"
#include "text.h"
#include "menu.h"
#include "file.h"
#include "db.h"
#include "view.h"
#include "view_sync.h"
#include "backend.h"
#include "ztore.h"

static struct view_sync view;

static void renderdefault()
{

    view.status.text.content = "Please wait...";

    menu_disable(&view.menu, 0);
    text_renderbox(&view.status, TEXT_COLOR_NORMAL);
    menu_render(&view.menu);

}

static void renderworking()
{

    view.status.text.content = "Syncing...";

    menu_enable(&view.menu, 0);
    text_renderbox(&view.status, TEXT_COLOR_NORMAL);
    menu_render(&view.menu);

}

static void rendercomplete()
{

    view.status.text.content = "Sync complete!";

    menu_disable(&view.menu, 0);
    text_renderbox(&view.status, TEXT_COLOR_NORMAL);
    menu_render(&view.menu);

}

static void renderfail()
{

    view.status.text.content = "Sync failed!";

    menu_disable(&view.menu, 0);
    text_renderbox(&view.status, TEXT_COLOR_NORMAL);
    menu_render(&view.menu);

}

static int sync(void *arg)
{

    struct db_remotelist remotelist;
    unsigned int i;

    ztore_setmode(renderworking);
    db_loadremotes(&remotelist);

    for (i = 0; i < remotelist.count; i++)
    {

        struct db_remote *remote = &remotelist.items[i];

        if (file_downloadremote(remote->url, remote->id))
            db_sync(remote);

        file_removeremote(remote->id);

    }

    ztore_setmode(rendercomplete);
    db_freeremotes(&remotelist);

    return 0;

}

static void load()
{

    void *syncthread;

    ztore_setmode(renderdefault);

    syncthread = backend_createthread(sync, NULL);

    if (!syncthread)
        ztore_setmode(renderfail);

}

static void keydown(unsigned int key)
{

    menu_keydown(&view.menu, key);

    switch (key)
    {

    case KEY_B:
        view_quit(&view.base);

        break;

    }

}

static void menu_onselect()
{

    switch (view.menu.currentitem)
    {

    case 0:
        break;

    }

}

struct view_sync *view_sync_setup(unsigned int w, unsigned int h)
{

    view_init(&view.base, load, renderdefault, keydown);
    text_init(&view.status.text, 0);
    box_init(&view.status.box, 0, 0, w, (4 * RENDER_ROWHEIGHT) + (2 * RENDER_PADDING));
    menu_init(&view.menu, view.menuitems, 1);
    menu_inititem(&view.menuitems[0], "Cancel");
    menu_disable(&view.menu, 0);
    menu_setrow(&view.menu, 0);
    box_init(&view.menu.box, 0, h - (view.menu.total * RENDER_ROWHEIGHT) - (2 * RENDER_PADDING), w, (view.menu.total * RENDER_ROWHEIGHT) + (2 * RENDER_PADDING));

    view.menu.onselect = menu_onselect;

    return &view;

}

