#include <stdlib.h>
#include <stdio.h>
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

    text_renderbox(&view.status, TEXT_COLOR_NORMAL, TEXT_ALIGN_LEFT, "Please wait...");

}

static void renderdownloading()
{

    char progress[128];

    snprintf(progress, 128, "Downloading...\n\nProgress: %d%%\nTotal bytes: %dKB", view.percentage, view.totalbytes);

    text_renderbox(&view.status, TEXT_COLOR_NORMAL, TEXT_ALIGN_LEFT, progress);
    menu_render(&view.menu);

}

static void rendercomplete()
{

    text_renderbox(&view.status, TEXT_COLOR_NORMAL, TEXT_ALIGN_LEFT, "Sync complete!\n\nPress B to go back.");

}

static void renderfail()
{

    text_renderbox(&view.status, TEXT_COLOR_NORMAL, TEXT_ALIGN_LEFT, "Sync failed!\n\nPress B to go back.");

}

static void keydownoff(unsigned int key)
{

}

static void keydownback(unsigned int key)
{

    switch (key)
    {

    case KEY_B:
        view_quit(&view.base);

        break;

    }

}

static void keydowndownloading(unsigned int key)
{

    menu_keydown(&view.menu, key);

}

static unsigned int downloadnotify(unsigned int totalbytes, unsigned int percentage)
{

    view.totalbytes = totalbytes;
    view.percentage = percentage;

    ztore_redraw();

    return !view.abortdownload;

}

static void sync()
{

    struct db_remotelist remotelist;
    unsigned int status = 0;
    unsigned int i;

    db_loadremotes(&remotelist);

    view_setmode(&view.base, renderdownloading, keydowndownloading);
    downloadnotify(0, 0);

    for (i = 0; i < remotelist.count; i++)
    {

        struct db_remote *remote = &remotelist.items[i];


        if (file_downloadremote(remote->url, remote->id, downloadnotify))
            status = db_sync(remote);

        file_removeremote(remote->id);

    }

    db_freeremotes(&remotelist);

    if (status)
        view_setmode(&view.base, rendercomplete, keydownback);
    else
        view_setmode(&view.base, renderfail, keydownback);
 
    ztore_redraw();

}

static void load()
{

    view.abortdownload = 0;

    view_setmode(&view.base, renderdefault, keydownoff);
    sync();

}

static void menu_onselect()
{

    switch (view.menu.currentitem)
    {

    case 0:
        view.abortdownload = 1;

        break;

    }

}

struct view_sync *view_sync_setup(unsigned int w, unsigned int h)
{

    view_init(&view.base, load, renderdefault, keydownoff);
    text_init(&view.status.text, 0);
    box_init(&view.status.box, 0, 0, w, (4 * RENDER_ROWHEIGHT) + (2 * RENDER_PADDING));
    menu_init(&view.menu, view.menuitems, 1);
    menu_inititem(&view.menuitems[0], "Cancel", 0);
    menu_setrow(&view.menu, 0);
    box_init(&view.menu.box, 0, h - (view.menu.total * RENDER_ROWHEIGHT) - (2 * RENDER_PADDING), w, (view.menu.total * RENDER_ROWHEIGHT) + (2 * RENDER_PADDING));

    view.menu.onselect = menu_onselect;

    return &view;

}

