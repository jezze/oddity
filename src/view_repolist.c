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

static struct view view;
static struct view *applistview;
static struct menu menu;
static struct box menubox;
static struct menuitem menuitems[4];
static char all[16];
static char new[16];
static char updated[16];
static char installed[16];
static struct db_applist applist;

static void render(void)
{

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

static void load(void)
{

    snprintf(all, 16, "%u items", db_countapps());
    snprintf(new, 16, "%u items", db_countappswithstate(1));
    snprintf(updated, 16, "%u items", db_countappswithstate(2));
    snprintf(installed, 16, "%u items", db_countappswithstate(3));

    menu.items[0].info = all;
    menu.items[1].info = installed;
    menu.items[2].info = new;
    menu.items[3].info = updated;

    ztore_setview(render, button);

}

static void menu_onselect(void)
{

    db_freeapps(&applist);

    switch (menu.currentitem)
    {

    case 0:
        db_loadapps(&applist);

        break;

    case 1:
        db_loadappswithstate(&applist, 3);

        break;

    case 2:
        db_loadappswithstate(&applist, 1);

        break;

    case 3:
        db_loadappswithstate(&applist, 2);

        break;

    }

    view_applist_setlist(&applist);
    view_load(applistview, &view);

}

struct view *view_repolist_setup(unsigned int w, unsigned int h)
{

    view_init(&view, load);
    box_init(&menubox);
    box_setpartsize(&menubox, w, h, 0, 0, 1, 1);
    menu_init(&menu, menuitems, 4);
    menu_inititem(&menuitems[0], "All", "0 items");
    menu_inititem(&menuitems[1], "Installed", "0 items");
    menu_inititem(&menuitems[2], "New", "0 items");
    menu_inititem(&menuitems[3], "Updated", "0 items");
    menu_setrow(&menu, 0);

    menu.onselect = menu_onselect;
    applistview = view_applist_setup(w, h);

    return &view;

}

