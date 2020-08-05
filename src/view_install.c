#include <stdlib.h>
#include <stdio.h>
#include "define.h"
#include "box.h"
#include "text.h"
#include "menu.h"
#include "file.h"
#include "db.h"
#include "view.h"
#include "view_install.h"
#include "ztore.h"

static struct view_install view;

static void renderdefault(void)
{

    text_render(&view.statusbox, TEXT_COLOR_NORMAL, TEXT_ALIGN_LEFT, "Please wait...");

}

static void renderpreparing(void)
{

    text_render(&view.statusbox, TEXT_COLOR_NORMAL, TEXT_ALIGN_LEFT, "Preparing...");

}

static void renderdownloading(void)
{

    char progress[128];

    snprintf(progress, 128, "Downloading %s...\n\nProgress: %d%%\nTotal bytes: %dKB", view.app->name, view.percentage, view.totalbytes);
    text_render(&view.statusbox, TEXT_COLOR_NORMAL, TEXT_ALIGN_LEFT, progress);
    menu_render(&view.menu, &view.menubox);

}

static void renderinstalling(void)
{

    text_render(&view.statusbox, TEXT_COLOR_NORMAL, TEXT_ALIGN_LEFT, "Installing...");

}

static void rendercomplete(void)
{

    text_render(&view.statusbox, TEXT_COLOR_NORMAL, TEXT_ALIGN_LEFT, "Installation complete!\n\nPress B to go back.");

}

static void renderfail(void)
{

    text_render(&view.statusbox, TEXT_COLOR_NORMAL, TEXT_ALIGN_LEFT, "Installation failed!\n\nPress B to go back.");

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

static void updatestates(struct db_package *package)
{

    view.app->state = 3;

    db_saveappstate(view.app);

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

static unsigned int downloadnotify(unsigned int totalbytes, unsigned int percentage)
{

    view.totalbytes = totalbytes;
    view.percentage = percentage;

    ztore_redraw(&view.base);

    return !view.abortdownload;

}

static unsigned int doinstall(struct db_packagelist *packagelist)
{

    if (!packagelist->count)
        return 0;

    ztore_setview(renderpreparing, keydownoff);
    ztore_redraw(&view.base);

    if (verifypackages(packagelist))
        return 1;

    ztore_setview(renderdownloading, keydowndownloading);
    downloadnotify(0, 0);

    if (!file_downloadpackage(packagelist->items[0].name, downloadnotify))
    {

        file_removepackage(packagelist->items[0].name);

        return 0;

    }

    ztore_setview(renderinstalling, keydownoff);
    ztore_redraw(&view.base);

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

    db_loadpackagesfromapp(&packagelist, view.app);

    if (doinstall(&packagelist))
        ztore_setview(rendercomplete, keydownback);
    else
        ztore_setview(renderfail, keydownback);

    ztore_redraw(&view.base);
    db_freepackages(&packagelist);

}

static void load(void)
{

    view.abortdownload = 0;

    ztore_setview(renderdefault, keydownoff);
    install();

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

struct view_install *view_install_setup(unsigned int w, unsigned int h)
{

    view_init(&view.base, load);
    box_init(&view.statusbox);
    box_init(&view.menubox);
    box_setpartsize(&view.statusbox, w / 10, h / 10, 0, 0, 10, 6);
    box_setpartsize(&view.menubox, w / 10, h / 10, 0, 6, 10, 4);
    menu_init(&view.menu, view.menuitems, 1);
    menu_inititem(&view.menuitems[0], "Cancel", 0);
    menu_setrow(&view.menu, 0);

    view.menu.onselect = menu_onselect;

    return &view;

}

