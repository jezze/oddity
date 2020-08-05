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
#include "ztore.h"

struct view_sync
{

    struct view base;
    struct box statusbox;
    struct menu menu;
    struct box menubox;
    struct menuitem menuitems[1];
    unsigned int percentage;
    unsigned int totalbytes;
    unsigned int abortdownload;

};

static struct view_sync view;

static void renderdefault(void)
{

    text_render(&view.statusbox, TEXT_COLOR_NORMAL, TEXT_ALIGN_LEFT, "Please wait...");

}

static void renderdownloading(void)
{

    char progress[128];

    snprintf(progress, 128, "Downloading...\n\nProgress: %d%%\nTotal bytes: %dKB", view.percentage, view.totalbytes);
    text_render(&view.statusbox, TEXT_COLOR_NORMAL, TEXT_ALIGN_LEFT, progress);
    menu_render(&view.menu, &view.menubox);

}

static void rendercomplete(void)
{

    text_render(&view.statusbox, TEXT_COLOR_NORMAL, TEXT_ALIGN_LEFT, "Sync complete!\n\nPress B to go back.");

}

static void renderfail(void)
{

    text_render(&view.statusbox, TEXT_COLOR_NORMAL, TEXT_ALIGN_LEFT, "Sync failed!\n\nPress B to go back.");

}

static void buttonoff(unsigned int key)
{

}

static void buttonback(unsigned int key)
{

    switch (key)
    {

    case KEY_B:
        view_quit(&view.base);

        break;

    }

}

static void buttondownloading(unsigned int key)
{

    menu_button(&view.menu, key);

}

static unsigned int downloadnotify(unsigned int totalbytes, unsigned int percentage)
{

    view.totalbytes = totalbytes;
    view.percentage = percentage;

    ztore_redraw();

    return !view.abortdownload;

}

static void sync(void)
{

    struct db_remotelist remotelist;
    unsigned int status = 0;
    unsigned int i;

    db_loadremotes(&remotelist);

    ztore_setview(renderdownloading, buttondownloading);
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
        ztore_setview(rendercomplete, buttonback);
    else
        ztore_setview(renderfail, buttonback);
 
    ztore_redraw();

}

static void load(void)
{

    view.abortdownload = 0;

    ztore_setview(renderdefault, buttonoff);
    sync();

}

static void menu_onselect(void)
{

    switch (view.menu.currentitem)
    {

    case 0:
        view.abortdownload = 1;

        break;

    }

}

struct view *view_sync_setup(unsigned int w, unsigned int h)
{

    view_init(&view.base, load);
    box_init(&view.statusbox);
    box_init(&view.menubox);
    box_setpartsize(&view.statusbox, w / 10, h / 10, 0, 0, 10, 8);
    box_setpartsize(&view.menubox, w / 10, h / 10, 0, 8, 10, 2);
    menu_init(&view.menu, view.menuitems, 1);
    menu_inititem(&view.menuitems[0], "Cancel", 0);
    menu_setrow(&view.menu, 0);

    view.menu.onselect = menu_onselect;

    return &view.base;

}

