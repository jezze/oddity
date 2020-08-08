#include <stdlib.h>
#include <stdio.h>
#include "define.h"
#include "box.h"
#include "text.h"
#include "menu.h"
#include "file.h"
#include "db.h"
#include "view.h"
#include "ztore.h"

static struct view view;
static struct box statusbox;
static struct menu menu;
static struct box menubox;
static struct menuitem menuitems[1];
static unsigned int percentage;
static unsigned int totalbytes;
static unsigned int abortdownload;

static void place(unsigned int w, unsigned int h)
{

    box_setpartsize(&statusbox, w / 10, h / 10, 0, 0, 10, 8);
    box_setpartsize(&menubox, w / 10, h / 10, 0, 8, 10, 2);

}

/*
static void renderdefault(void)
{

    text_render(&statusbox, TEXT_COLOR_NORMAL, TEXT_ALIGN_LEFT, "Please wait...");

}
*/

static void renderdownloading(void)
{

    char progress[128];

    snprintf(progress, 128, "Downloading...\n\nProgress: %d%%\nTotal bytes: %dKB", percentage, totalbytes);
    text_render(&statusbox, TEXT_COLOR_NORMAL, TEXT_ALIGN_LEFT, progress);
    menu_render(&menu, &menubox);

    totalbytes++;

}

/*
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
*/

static void buttondownloading(unsigned int key)
{

    menu_button(&menu, key);

    switch (key)
    {

    case KEY_B:
        view_quit("sync");

        break;

    }

}

static unsigned int downloadnotify(unsigned int t, unsigned int p)
{

    totalbytes = t;
    percentage = p;

    return !abortdownload;

}

/*
static void sync(void)
{

    struct db_remotelist remotelist;
    unsigned int status = 0;
    unsigned int i;

    ztore_setview(place, renderdownloading, buttondownloading);

    db_loadremotes(&remotelist);

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
 
}
*/

static void load(void)
{

    abortdownload = 0;

    downloadnotify(0, 0);
    ztore_setview(place, renderdownloading, buttondownloading);
    /*
    sync();
    */

}

static void event(char *key, void *value)
{

}

static void menu_onselect(unsigned int index)
{

    switch (index)
    {

    case 0:
        abortdownload = 1;

        view_quit("sync");

        break;

    }

}

void view_sync_setup(void)
{

    view_init(&view, load, event);
    box_init(&statusbox);
    box_init(&menubox);
    menu_init(&menu, menuitems, 1, menu_onselect);
    menu_inititem(&menuitems[0], "Cancel", 0);
    menu_setrow(&menu, 0);
    view_register("sync", &view);

}

