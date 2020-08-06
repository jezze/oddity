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

static struct view view;
static struct box statusbox;
static struct menu menu;
static struct box menubox;
static struct menuitem menuitems[1];
unsigned int percentage;
unsigned int totalbytes;
unsigned int abortdownload;

static void place(unsigned int w, unsigned int h)
{

    box_setpartsize(&statusbox, w / 10, h / 10, 0, 0, 10, 8);
    box_setpartsize(&menubox, w / 10, h / 10, 0, 8, 10, 2);

}

static void renderdefault(void)
{

    text_render(&statusbox, TEXT_COLOR_NORMAL, TEXT_ALIGN_LEFT, "Please wait...");

}

static void renderdownloading(void)
{

    char progress[128];

    snprintf(progress, 128, "Downloading...\n\nProgress: %d%%\nTotal bytes: %dKB", percentage, totalbytes);
    text_render(&statusbox, TEXT_COLOR_NORMAL, TEXT_ALIGN_LEFT, progress);
    menu_render(&menu, &menubox);

}

static void rendercomplete(void)
{

    text_render(&statusbox, TEXT_COLOR_NORMAL, TEXT_ALIGN_LEFT, "Sync complete!\n\nPress B to go back.");

}

static void renderfail(void)
{

    text_render(&statusbox, TEXT_COLOR_NORMAL, TEXT_ALIGN_LEFT, "Sync failed!\n\nPress B to go back.");

}

static void buttonoff(unsigned int key)
{

}

static void buttonback(unsigned int key)
{

    switch (key)
    {

    case KEY_B:
        view_quit(&view);

        break;

    }

}

static void buttondownloading(unsigned int key)
{

    menu_button(&menu, key);

}

static unsigned int downloadnotify(unsigned int t, unsigned int p)
{

    totalbytes = t;
    percentage = p;

    ztore_redraw();

    return !abortdownload;

}

static void sync(void)
{

    struct db_remotelist remotelist;
    unsigned int status = 0;
    unsigned int i;

    db_loadremotes(&remotelist);

    ztore_setview(place, renderdownloading, buttondownloading);
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
        ztore_setview(place, rendercomplete, buttonback);
    else
        ztore_setview(place, renderfail, buttonback);
 
    ztore_redraw();

}

static void load(void)
{

    abortdownload = 0;

    ztore_setview(place, renderdefault, buttonoff);
    sync();

}

static void menu_onselect(void)
{

    switch (menu.currentitem)
    {

    case 0:
        abortdownload = 1;

        break;

    }

}

struct view *view_sync_setup(void)
{

    view_init(&view, load);
    box_init(&statusbox);
    box_init(&menubox);
    menu_init(&menu, menuitems, 1);
    menu_inititem(&menuitems[0], "Cancel", 0);
    menu_setrow(&menu, 0);

    menu.onselect = menu_onselect;

    return &view;

}

