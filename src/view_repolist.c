#include <stdlib.h>
#include <stdio.h>
#include "define.h"
#include "box.h"
#include "text.h"
#include "list.h"
#include "menu.h"
#include "db.h"
#include "view.h"
#include "ztore.h"

static struct view view;
static struct menu menu;
static struct box menubox;
static struct menuitem menuitems[4];
static char all[16];
static char new[16];
static char updated[16];
static char installed[16];
static struct db_applist applist;

static void place(unsigned int w, unsigned int h)
{

    box_setpartsize(&menubox, w, h, 0, 0, 1, 1);

}

static void render(unsigned int ticks)
{

    menu_render(&menu, &menubox);

}

static void button(unsigned int key)
{

    menu_button(&menu, key);

    switch (key)
    {

    case KEY_B:
        view_quit("repolist");

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

    ztore_setview(place, render, button);

}

static void config(char *key, void *value)
{

}

static void menu_onselect(unsigned int index)
{

    db_freeapps(&applist);

    switch (index)
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

    view_config("applist", "list", &applist);
    view_load("applist", "repolist");

}

void view_repolist_setup(void)
{

    view_init(&view, load, config);
    box_init(&menubox);
    menu_init(&menu, menuitems, 4, menu_onselect);
    menu_inititem(&menuitems[0], "All", "0 items");
    menu_inititem(&menuitems[1], "Installed", "0 items");
    menu_inititem(&menuitems[2], "New", "0 items");
    menu_inititem(&menuitems[3], "Updated", "0 items");
    menu_setrow(&menu, 0);
    view_register("repolist", &view);

}

