#include <stdlib.h>
#include <string.h>
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
static struct db_app *app;
static struct box statusbox;
static struct menu menu;
static struct box menubox;
static struct menuitem menuitems[1];
static unsigned int percentage;
static unsigned int totalbytes;
static unsigned int abortdownload;

static void place(unsigned int w, unsigned int h)
{

    box_setpartsize(&statusbox, w / 10, h / 10, 0, 0, 10, 6);
    box_setpartsize(&menubox, w / 10, h / 10, 0, 6, 10, 4);

}

static void renderdefault(unsigned int ticks)
{

    text_render(&statusbox, TEXT_COLOR_NORMAL, TEXT_ALIGN_LEFT, "Please wait...");

}

static void renderpreparing(unsigned int ticks)
{

    text_render(&statusbox, TEXT_COLOR_NORMAL, TEXT_ALIGN_LEFT, "Preparing...");

}

static void renderdownloading(unsigned int ticks)
{

    char progress[128];

    snprintf(progress, 128, "Downloading %s...\n\nProgress: %d%%\nTotal bytes: %dKB", app->name, percentage, totalbytes);
    text_render(&statusbox, TEXT_COLOR_NORMAL, TEXT_ALIGN_LEFT, progress);
    menu_render(&menu, &menubox);

}

static void renderinstalling(unsigned int ticks)
{

    text_render(&statusbox, TEXT_COLOR_NORMAL, TEXT_ALIGN_LEFT, "Installing...");

}

static void rendercomplete(unsigned int ticks)
{

    text_render(&statusbox, TEXT_COLOR_NORMAL, TEXT_ALIGN_LEFT, "Installation complete!\n\nPress B to go back.");

}

static void renderfail(unsigned int ticks)
{

    text_render(&statusbox, TEXT_COLOR_NORMAL, TEXT_ALIGN_LEFT, "Installation failed!\n\nPress B to go back.");

}

static void buttonoff(unsigned int key)
{

}

static void buttonback(unsigned int key)
{

    switch (key)
    {

    case KEY_B:
        view_quit("install");

        break;

    }

}

static void buttondownloading(unsigned int key)
{

    menu_button(&menu, key);

}

static void updatestates(struct db_package *package)
{

    app->state = 3;

    db_saveappstate(app);

    package->state = 3;

    db_savepackagestate(package);

}

static unsigned int verifypackage(struct db_package *package)
{

    char path[128];

    file_getpackagepath(path, 128, package->name);

    if (file_exist(path))
    {

        if (file_matchsha1(path, package->sha1))
            return 1;

        file_removepackage(package->name);

    }

    return 0;

}

static unsigned int verifypackages(struct db_packagelist *packagelist)
{

    unsigned int i;

    for (i = 0; i < packagelist->count; i++)
    {

        if (verifypackage(&packagelist->items[i]))
        {

            updatestates(&packagelist->items[i]);

            return 1;

        }

    }

    return 0;

}

static unsigned int downloadnotify(unsigned int t, unsigned int p)
{

    totalbytes = t;
    percentage = p;

    return !abortdownload;

}

static unsigned int doinstall(struct db_packagelist *packagelist)
{

    if (!packagelist->count)
        return 0;

    ztore_setview(place, renderpreparing, buttonoff);

    if (verifypackages(packagelist))
        return 1;

    ztore_setview(place, renderdownloading, buttondownloading);
    downloadnotify(0, 0);

    if (!file_downloadpackage(packagelist->items[0].name, downloadnotify))
    {

        file_removepackage(packagelist->items[0].name);

        return 0;

    }

    ztore_setview(place, renderinstalling, buttonoff);

    if (verifypackage(&packagelist->items[0]))
    {

        updatestates(&packagelist->items[0]);

        return 1;

    }

    return 0;

}

static void install(void)
{

    struct db_packagelist packagelist;

    db_loadpackagesfromapp(&packagelist, app);

    if (doinstall(&packagelist))
        ztore_setview(place, rendercomplete, buttonback);
    else
        ztore_setview(place, renderfail, buttonback);

    db_freepackages(&packagelist);

}

static void load(void)
{

    abortdownload = 0;

    ztore_setview(place, renderdefault, buttonoff);
    install();

}

static void config(char *key, void *value)
{

    if (!strcmp(key, "app"))
        app = value;

}

static void menu_onselect(unsigned int index)
{

    switch (index)
    {

    case 0:
        abortdownload = 1;

        break;

    }

}

void view_install_setup(void)
{

    view_init(&view, load, config);
    box_init(&statusbox);
    box_init(&menubox);
    menu_init(&menu, menuitems, 1, menu_onselect);
    menu_inititem(&menuitems[0], "Cancel", 0);
    menu_setrow(&menu, 0);
    view_register("install", &view);

}

