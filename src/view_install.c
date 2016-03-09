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

static void renderdefault()
{

    text_renderbox(&view.status, view.statusbox.x, view.statusbox.y, view.statusbox.w, view.statusbox.h, TEXT_COLOR_NORMAL, TEXT_ALIGN_LEFT, "Please wait...");

}

static void renderpreparing()
{

    text_renderbox(&view.status, view.statusbox.x, view.statusbox.y, view.statusbox.w, view.statusbox.h, TEXT_COLOR_NORMAL, TEXT_ALIGN_LEFT, "Preparing...");

}

static void renderdownloading()
{

    char progress[128];

    snprintf(progress, 128, "Downloading %s...\n\nProgress: %d%%\nTotal bytes: %dKB", view.app->name, view.percentage, view.totalbytes);
    text_renderbox(&view.status, view.statusbox.x, view.statusbox.y, view.statusbox.w, view.statusbox.h, TEXT_COLOR_NORMAL, TEXT_ALIGN_LEFT, progress);
    menu_render(&view.menu);

}

static void renderinstalling()
{

    text_renderbox(&view.status, view.statusbox.x, view.statusbox.y, view.statusbox.w, view.statusbox.h, TEXT_COLOR_NORMAL, TEXT_ALIGN_LEFT, "Installing...");

}

static void rendercomplete()
{

    text_renderbox(&view.status, view.statusbox.x, view.statusbox.y, view.statusbox.w, view.statusbox.h, TEXT_COLOR_NORMAL, TEXT_ALIGN_LEFT, "Installation complete!\n\nPress B to go back.");

}

static void renderfail()
{

    text_renderbox(&view.status, view.statusbox.x, view.statusbox.y, view.statusbox.w, view.statusbox.h, TEXT_COLOR_NORMAL, TEXT_ALIGN_LEFT, "Installation failed!\n\nPress B to go back.");

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

    view_redraw();

    return !view.abortdownload;

}

static unsigned int doinstall(struct db_packagelist *packagelist)
{

    if (!packagelist->count)
        return 0;

    view_setmode(&view.base, renderpreparing, keydownoff);
    view_redraw();

    if (verifypackages(packagelist))
        return 1;

    view_setmode(&view.base, renderdownloading, keydowndownloading);
    downloadnotify(0, 0);

    if (!file_downloadpackage(packagelist->items[0].name, downloadnotify))
    {

        file_removepackage(packagelist->items[0].name);

        return 0;

    }

    view_setmode(&view.base, renderinstalling, keydownoff);
    view_redraw();

    if (verifypackage(&packagelist->items[0]))
    {

        updatestates(&packagelist->items[0]);

        return 1;

    }

    return 0;

}

static void install()
{

    struct db_packagelist packagelist;

    db_loadpackagesfromapp(&packagelist, view.app);

    if (doinstall(&packagelist))
        view_setmode(&view.base, rendercomplete, keydownback);
    else
        view_setmode(&view.base, renderfail, keydownback);

    view_redraw();
    db_freepackages(&packagelist);

}

static void load()
{

    view.onload();
    view.abortdownload = 0;

    view_setmode(&view.base, renderdefault, keydownoff);
    install();

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

struct view_install *view_install_setup(unsigned int w, unsigned int h)
{

    view_init(&view.base, load, renderdefault, keydownoff);
    text_init(&view.status.text);
    box_init(&view.statusbox, 0, 0, w, (4 * RENDER_ROWHEIGHT) + (2 * RENDER_PADDING));
    menu_init(&view.menu, view.menuitems, 1);
    menu_inititem(&view.menuitems[0], "Cancel", 0);
    menu_setrow(&view.menu, 0);
    box_init(&view.menu.box, 0, h - (view.menu.total * RENDER_ROWHEIGHT) - (2 * RENDER_PADDING), w, (view.menu.total * RENDER_ROWHEIGHT) + (2 * RENDER_PADDING));

    view.menu.onselect = menu_onselect;

    return &view;

}

