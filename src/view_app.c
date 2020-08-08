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

static struct view view;
static struct view *installview;
static struct view *uninstallview;
static struct box titlebox;
static struct box shortbox;
static struct menu menu;
static struct box menubox;
static struct menuitem menuitems[3];
static struct db_app *app;

static void place(unsigned int w, unsigned int h)
{

    box_setpartsize(&titlebox, w / 10, h / 10, 0, 0, 10, 2);
    box_setpartsize(&shortbox, w / 10, h / 10, 0, 1, 10, 6);
    box_setpartsize(&menubox, w / 10, h / 10, 0, 6, 10, 4);

}

static void render(void)
{

    text_render(&titlebox, TEXT_COLOR_TITLE, TEXT_ALIGN_LEFT, app->name);
    text_render(&shortbox, TEXT_COLOR_NORMAL, TEXT_ALIGN_LEFT, app->shortdescription);
    menu_render(&menu, &menubox);

}

static void button(unsigned int key)
{

    menu_button(&menu, key);

    switch (key)
    {

    case KEY_B:
        view_quit(&view);

        break;

    }

}

static void updatestate(void)
{

    if (app->state == 1 || app->state == 2)
    {

        app->state = 0;

        db_saveappstate(app);

    }

}

static void load(void)
{

    if (app->state == 3)
    {

        menu_enable(&menu, 0);
        menu_disable(&menu, 1);
        menu_enable(&menu, 2);
        menu_setrow(&menu, 0);

    }

    else
    {

        menu_disable(&menu, 0);
        menu_enable(&menu, 1);
        menu_disable(&menu, 2);
        menu_setrow(&menu, 1);

    }

    updatestate();
    ztore_setview(place, render, button);

}

static void runpackage(void)
{

    struct db_packagelist packagelist;
    unsigned int i;

    db_loadpackagesfromapp(&packagelist, app);

    for (i = 0; i < packagelist.count; i++)
    {

        if (packagelist.items[i].state != 3)
            continue;

        ztore_exec(packagelist.items[i].name);

        break;

    }

    db_freepackages(&packagelist);

}

static void menu_onselect(unsigned int index)
{

    switch (index)
    {

    case 0:
        runpackage();

        break;

    case 1:
        view_install_setapp(app);
        view_load(installview, &view);

        break;

    case 2:
        view_uninstall_setapp(app);
        view_load(uninstallview, &view);

        break;

    }

}

void view_app_setapp(struct db_app *item)
{

    app = item;

}

struct view *view_app_setup(void)
{

    view_init(&view, load);
    box_init(&titlebox);
    box_init(&shortbox);
    box_init(&menubox);
    menu_init(&menu, menuitems, 3);
    menu_inititem(&menuitems[0], "Run", 0);
    menu_inititem(&menuitems[1], "Install", 0);
    menu_inititem(&menuitems[2], "Uninstall", 0);
    menu_setrow(&menu, 0);

    menu.onselect = menu_onselect;
    installview = view_install_setup();
    uninstallview = view_uninstall_setup();

    return &view;

}

