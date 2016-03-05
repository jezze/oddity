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
static char *status[4] = {
    "Please wait...",
    "Syncing...",
    "Sync complete!",
    "Sync failed!"
};

static void changestate(unsigned int state)
{

    view.state = state;

    ztore_redraw();

}

static int sync(void *arg)
{

    struct db_remotelist remotelist;
    unsigned int i;

    menu_enable(&view.menu, 0);
    changestate(1);
    db_loadremotes(&remotelist);

    for (i = 0; i < remotelist.count; i++)
    {

        file_downloadremote(remotelist.items[i].url, remotelist.items[i].id);
        db_sync(&remotelist.items[i]);
        file_removeremote(remotelist.items[i].id);
        db_freeremote(&remotelist.items[i]);

    }

    menu_disable(&view.menu, 0);
    changestate(2);

    return 0;

}

static void load()
{

    void *syncthread;

    changestate(0);

    syncthread = backend_createthread(sync, NULL);

    if (!syncthread)
        changestate(3);

}

static void render()
{

    view.status.text.content = status[view.state];

    text_renderbox(&view.status, TEXT_COLOR_NORMAL);
    menu_render(&view.menu);

}

static void keydown(unsigned int key)
{

    switch (key)
    {

    case KEY_A:
        if (!menu_isactive(&view.menu, view.menu.currentitem))
            break;

        switch (view.menu.currentitem)
        {

        case 0:
            break;

        }

        break;

    case KEY_B:
        view_quit(&view.base);

        break;

    }

}

struct view_sync *view_sync_setup(unsigned int w, unsigned int h)
{

    view_init(&view.base, load, render, keydown);
    text_init(&view.status.text, status[0]);
    box_init(&view.status.box, 0, 0, w, (4 * RENDER_ROWHEIGHT) + (2 * RENDER_PADDING));
    menu_init(&view.menu, view.menuitems, 1);
    menu_inititem(&view.menuitems[0], "Cancel");
    menu_disable(&view.menu, 0);
    menu_setrow(&view.menu, 0);
    box_init(&view.menu.box, 0, h - (view.menu.total * RENDER_ROWHEIGHT) - (2 * RENDER_PADDING), w, (view.menu.total * RENDER_ROWHEIGHT) + (2 * RENDER_PADDING));

    return &view;

}

