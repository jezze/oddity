#include <stdlib.h>
#include <stdio.h>
#include "define.h"
#include "box.h"
#include "text.h"
#include "menu.h"
#include "file.h"
#include "db.h"
#include "view.h"
#include "view_uninstall.h"
#include "ztore.h"

static struct view_uninstall view;

static void renderconfirm()
{

    text_render(&view.statusbox, TEXT_COLOR_NORMAL, TEXT_ALIGN_LEFT, "Are you sure you want to uninstall?");
    menu_render(&view.menu, &view.menubox);

}

static void renderuninstalling()
{

    char progress[128];

    snprintf(progress, 128, "Uninstalling %s...", view.app->name);
    text_render(&view.statusbox, TEXT_COLOR_NORMAL, TEXT_ALIGN_LEFT, progress);

}

static void rendercomplete()
{

    text_render(&view.statusbox, TEXT_COLOR_NORMAL, TEXT_ALIGN_LEFT, "Uninstall complete!\n\nPress B to go back.");

}

static void renderfail()
{

    text_render(&view.statusbox, TEXT_COLOR_NORMAL, TEXT_ALIGN_LEFT, "Uninstall failed!\n\nPress B to go back.");

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

static void keydownconfirm(unsigned int key)
{

    menu_keydown(&view.menu, key);

    switch (key)
    {

    case KEY_B:
        view_quit(&view.base);

        break;

    }

}

static void updatestates(struct db_package *package)
{

    view.app->state = 0;

    db_saveappstate(view.app);

    package->state = 0;

    db_savepackagestate(package);

}

static unsigned int douninstall(struct db_packagelist *packagelist)
{

    unsigned int i;

    for (i = 0; i < packagelist->count; i++)
    {

        char path[128];

        updatestates(&packagelist->items[i]);
        file_getpackagepath(path, 128, packagelist->items[i].name);

        if (file_exist(path))
            file_removepackage(packagelist->items[i].name);

    }

    return 1;

}

static void uninstall()
{

    struct db_packagelist packagelist;

    view_setmode(&view.base, renderuninstalling, keydownoff);
    view_redraw();

    db_loadpackagesfromapp(&packagelist, view.app);

    if (douninstall(&packagelist))
        view_setmode(&view.base, rendercomplete, keydownback);
    else
        view_setmode(&view.base, renderfail, keydownback);

    view_redraw();
    db_freepackages(&packagelist);

}

static void load()
{

    view.onload();

    view_setmode(&view.base, renderconfirm, keydownconfirm);

}

static void menu_onselect()
{

    switch (view.menu.currentitem)
    {

    case 0:
        uninstall();

        break;

    }

}

struct view_uninstall *view_uninstall_setup(unsigned int w, unsigned int h)
{

    view_init(&view.base, load, renderconfirm, keydownconfirm);
    box_init(&view.statusbox);
    box_init(&view.menubox);
    box_setpartsize(&view.statusbox, w / 10, h / 10, 0, 0, 10, 8);
    box_setpartsize(&view.menubox, w / 10, h / 10, 0, 8, 10, 2);
    menu_init(&view.menu, view.menuitems, 1);
    menu_inititem(&view.menuitems[0], "Yes, I am sure", 0);
    menu_setrow(&view.menu, 0);

    view.menu.onselect = menu_onselect;

    return &view;

}

