#include <stdlib.h>
#include "define.h"
#include "box.h"
#include "text.h"
#include "menu.h"
#include "db.h"
#include "view.h"
#include "view_applist.h"
#include "ztore.h"

static struct view view;
static struct view_applist *applistview;
static struct menu menu;
static struct menuitem menuitems[3];

static void show()
{

    ztore_flipview(&view);

}

static void render()
{

    menu_render(&menu);

}

static void keydown(unsigned int key)
{

    switch (key)
    {

    case KEY_UP:
        menu_prevrow(&menu);

        break;

    case KEY_DOWN:
        menu_nextrow(&menu);

        break;

    case KEY_A:
        switch (menu.currentitem)
        {

        case 0:
            applistview->base.show();

            break;

        case 1:
            applistview->base.show();

            break;

        case 2:
            applistview->base.show();

            break;

        }

        break;

    case KEY_B:
        view_quit(&view);

        break;

    }

}

static void applistview_onload(struct db_applist *applist)
{

    db_countapps(applist);

    applist->items = malloc(sizeof (struct db_app) * applist->count);

    db_loadapps(applist->items, 0, applist->count);

}

static void applistview_onunload(struct db_applist *applist)
{

    unsigned int i;

    for (i = 0; i < applist->count; i++)
    {

        free(applist->items[i].name);
        free(applist->items[i].shortdescription);

    }

    free(applist->items);

}

struct view *view_repolist_setup(unsigned int w, unsigned int h)
{

    view_init(&view, show, render, keydown);
    menu_init(&menu, menuitems, 3);
    menu_inititem(&menuitems[0], "All", MENUITEM_FLAG_NORMAL);
    menu_inititem(&menuitems[1], "Installed", MENUITEM_FLAG_NORMAL);
    menu_inititem(&menuitems[2], "Official", MENUITEM_FLAG_NORMAL);
    menu_setrow(&menu, 0);
    box_init(&menu.box, 0, 0, w, h);

    applistview = view_applist_setup(w, h);
    applistview->onload = applistview_onload;
    applistview->onunload = applistview_onunload;
    applistview->base.onquit = show;

    return &view;

}

