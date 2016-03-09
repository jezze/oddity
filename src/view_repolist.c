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

static struct view_repolist view;
static char all[16];
static char installed[16];

static void render()
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

static void load()
{

    snprintf(all, 16, "%u items", db_countapps());
    snprintf(installed, 16, "%u items", db_countinstalledapps());

    view.menu.items[0].info = all;
    view.menu.items[1].info = installed;

}

static void menu_onselect()
{

    switch (view.menu.currentitem)
    {

    case 0:
        view_load(&view.applistview->base);

        break;

    case 1:
        view_load(&view.applistview->base);

        break;

    }

}

static void applistview_onquit()
{

    view_load(&view.base);

}

static void applistview_onload()
{

    db_freeapps(&view.applistview->applist);

    switch (view.menu.currentitem)
    {

    case 0:
        db_loadapps(&view.applistview->applist);

        break;

    case 1:
        db_loadinstalledapps(&view.applistview->applist);

        break;

    }

}

struct view_repolist *view_repolist_setup(unsigned int w, unsigned int h)
{

    view_init(&view.base, load, render, keydown);
    box_init(&view.menubox, 0, 0, w, h);
    menu_init(&view.menu, view.menuitems, 2);
    menu_inititem(&view.menuitems[0], "All", "0 items");
    menu_inititem(&view.menuitems[1], "Installed", "0 items");
    menu_setrow(&view.menu, 0);

    view.menu.onselect = menu_onselect;
    view.applistview = view_applist_setup(w, h);
    view.applistview->onload = applistview_onload;
    view.applistview->base.onquit = applistview_onquit;

    return &view;

}

