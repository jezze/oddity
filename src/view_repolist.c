#include <stdlib.h>
#include <stdio.h>
#include "define.h"
#include "box.h"
#include "text.h"
#include "menu.h"
#include "db.h"
#include "view.h"
#include "view_applist.h"
#include "view_repolist.h"
#include "ztore.h"

struct view_repolist
{

    struct view base;
    struct view_applist *applistview;
    struct menu menu;
    struct box menubox;
    struct menuitem menuitems[4];

};

static struct view_repolist view;
static char all[16];
static char new[16];
static char updated[16];
static char installed[16];

static void render(void)
{

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

static void load(void)
{

    snprintf(all, 16, "%u items", db_countapps());
    snprintf(new, 16, "%u items", db_countappswithstate(1));
    snprintf(updated, 16, "%u items", db_countappswithstate(2));
    snprintf(installed, 16, "%u items", db_countappswithstate(3));

    view.menu.items[0].info = all;
    view.menu.items[1].info = installed;
    view.menu.items[2].info = new;
    view.menu.items[3].info = updated;

}

static void menu_onselect(void)
{

    switch (view.menu.currentitem)
    {

    case 0:
        view_load(&view.applistview->base);

        break;

    case 1:
        view_load(&view.applistview->base);

        break;

    case 2:
        view_load(&view.applistview->base);

        break;

    case 3:
        view_load(&view.applistview->base);

        break;

    }

}

static void applistview_onquit(void)
{

    view_load(&view.base);

}

static void applistview_preload(void)
{

    db_freeapps(&view.applistview->applist);

    switch (view.menu.currentitem)
    {

    case 0:
        db_loadapps(&view.applistview->applist);

        break;

    case 1:
        db_loadappswithstate(&view.applistview->applist, 3);

        break;

    case 2:
        db_loadappswithstate(&view.applistview->applist, 1);

        break;

    case 3:
        db_loadappswithstate(&view.applistview->applist, 2);

        break;

    }

}

struct view *view_repolist_setup(unsigned int w, unsigned int h)
{

    view_init(&view.base, load, render, keydown);
    box_init(&view.menubox);
    box_setpartsize(&view.menubox, w, h, 0, 0, 1, 1);
    menu_init(&view.menu, view.menuitems, 4);
    menu_inititem(&view.menuitems[0], "All", "0 items");
    menu_inititem(&view.menuitems[1], "Installed", "0 items");
    menu_inititem(&view.menuitems[2], "New", "0 items");
    menu_inititem(&view.menuitems[3], "Updated", "0 items");
    menu_setrow(&view.menu, 0);

    view.menu.onselect = menu_onselect;
    view.applistview = view_applist_setup(w, h);
    view.applistview->base.preload = applistview_preload;
    view.applistview->base.onquit = applistview_onquit;

    return &view.base;

}

