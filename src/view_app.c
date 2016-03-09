#include <stdlib.h>
#include <string.h>
#include "define.h"
#include "box.h"
#include "text.h"
#include "menu.h"
#include "file.h"
#include "db.h"
#include "view.h"
#include "view_app.h"
#include "view_install.h"
#include "view_uninstall.h"
#include "ztore.h"

static struct view_app view;

static void render()
{

    text_render(&view.titlebox, TEXT_COLOR_TITLE, TEXT_ALIGN_LEFT, view.app->name);
    text_render(&view.shortbox, TEXT_COLOR_NORMAL, TEXT_ALIGN_LEFT, view.app->shortdescription);
    menu_render(&view.menu, &view.menubox);

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

static void updatestate()
{

    if (view.app->state == 1 || view.app->state == 2)
    {

        view.app->state = 0;

        db_saveappstate(view.app);

    }

}

static void load()
{

    view.onload();

    if (view.app->state == 3)
    {

        menu_enable(&view.menu, 0);
        menu_disable(&view.menu, 1);
        menu_enable(&view.menu, 2);
        menu_setrow(&view.menu, 0);

    }

    else
    {

        menu_disable(&view.menu, 0);
        menu_enable(&view.menu, 1);
        menu_disable(&view.menu, 2);
        menu_setrow(&view.menu, 1);

    }

    updatestate();

}

static void runpackage()
{

    struct db_packagelist packagelist;
    unsigned int i;

    db_loadpackagesfromapp(&packagelist, view.app);

    for (i = 0; i < packagelist.count; i++)
    {

        if (packagelist.items[i].state != 3)
            continue;

        ztore_exec(packagelist.items[i].name);

        break;

    }

    db_freepackages(&packagelist);

}

static void menu_onselect()
{

    switch (view.menu.currentitem)
    {

    case 0:
        runpackage();

        break;

    case 1:
        view_load(&view.installview->base);

        break;

    case 2:
        view_load(&view.uninstallview->base);

        break;

    }

}

static void xinstallview_onquit()
{

    view_load(&view.base);

}

static void installview_onload()
{

    view.installview->app = view.app;

}

static void uninstallview_onload()
{

    view.uninstallview->app = view.app;

}

struct view_app *view_app_setup(unsigned int w, unsigned int h)
{

    view_init(&view.base, load, render, keydown);
    box_init(&view.titlebox);
    box_init(&view.shortbox);
    box_init(&view.menubox);
    box_setpartsize(&view.titlebox, w / 10, h / 10, 0, 0, 10, 2);
    box_setpartsize(&view.shortbox, w / 10, h / 10, 0, 1, 10, 6);
    box_setpartsize(&view.menubox, w / 10, h / 10, 0, 6, 10, 4);
    menu_init(&view.menu, view.menuitems, 3);
    menu_inititem(&view.menuitems[0], "Run", 0);
    menu_inititem(&view.menuitems[1], "Install", 0);
    menu_inititem(&view.menuitems[2], "Uninstall", 0);
    menu_setrow(&view.menu, 0);

    view.menu.onselect = menu_onselect;
    view.installview = view_install_setup(w, h);
    view.installview->base.onquit = xinstallview_onquit;
    view.installview->onload = installview_onload;
    view.uninstallview = view_uninstall_setup(w, h);
    view.uninstallview->base.onquit = xinstallview_onquit;
    view.uninstallview->onload = uninstallview_onload;

    return &view;

}

