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

static void load()
{

    view.onload();

    view.title.text.content = view.app->name;
    view.shortdescription.text.content = view.app->shortdescription;

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

}

static void render()
{

    text_renderbox(&view.title, TEXT_COLOR_TITLE);
    text_renderbox(&view.shortdescription, TEXT_COLOR_NORMAL);
    menu_render(&view.menu);

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

static void menu_onselect()
{

    switch (view.menu.currentitem)
    {

    case 0:
        runpackage();

        break;

    case 1:
        ztore_load(&view.installview->base);

        break;

    case 2:
        ztore_load(&view.uninstallview->base);

        break;

    }

}

static void xinstallview_onquit()
{

    ztore_load(&view.base);

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
    menu_init(&view.menu, view.menuitems, 3);
    menu_inititem(&view.menuitems[0], "Run");
    menu_inititem(&view.menuitems[1], "Install");
    menu_inititem(&view.menuitems[2], "Uninstall");
    menu_setrow(&view.menu, 0);
    box_init(&view.title.box, 0, 0, w, (1 * RENDER_ROWHEIGHT) + (2 * RENDER_PADDING));
    box_init(&view.shortdescription.box, 0, (1 * RENDER_ROWHEIGHT), w, (4 * RENDER_ROWHEIGHT) + (2 * RENDER_PADDING));
    box_init(&view.menu.box, 0, h - (view.menu.total * RENDER_ROWHEIGHT) - (2 * RENDER_PADDING), w, (view.menu.total * RENDER_ROWHEIGHT) + (2 * RENDER_PADDING));

    view.menu.onselect = menu_onselect;
    view.installview = view_install_setup(w, h);
    view.installview->base.onquit = xinstallview_onquit;
    view.installview->onload = installview_onload;
    view.uninstallview = view_uninstall_setup(w, h);
    view.uninstallview->base.onquit = xinstallview_onquit;
    view.uninstallview->onload = uninstallview_onload;

    return &view;

}

