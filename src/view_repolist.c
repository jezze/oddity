#include <stdlib.h>
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

static void load()
{

}

static void render()
{

    menu_render(&view.menu);

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
        ztore_load(&view.applistview->base);

        break;

    case 1:
        ztore_load(&view.applistview->base);

        break;

    }

}

static void applistview_onquit()
{

    ztore_load(&view.base);

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
    menu_init(&view.menu, view.menuitems, 2);
    menu_inititem(&view.menuitems[0], "All");
    menu_inititem(&view.menuitems[1], "Installed");
    menu_setrow(&view.menu, 0);
    box_init(&view.menu.box, 0, 0, w, h);

    view.menu.onselect = menu_onselect;
    view.applistview = view_applist_setup(w, h);
    view.applistview->onload = applistview_onload;
    view.applistview->base.onquit = applistview_onquit;

    return &view;

}

