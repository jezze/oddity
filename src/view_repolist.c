#include <stdlib.h>
#include "view.h"
#include "box.h"
#include "text.h"
#include "menu.h"
#include "backend.h"
#include "ztore.h"

static struct view view;
static struct view *applistview;
static struct menu menu;
static struct menuitem menuitems[3];

static void show()
{

}

static void hide()
{

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
            view_applist_config(0);
            ztore_flipview(applistview);

            break;

        case 1:
            view_applist_config(0);
            ztore_flipview(applistview);

            break;

        case 2:
            view_applist_config(0);
            ztore_flipview(applistview);

            break;

        }

        break;

    case KEY_B:
        ztore_flipview(view.parent);

        break;

    }

}

struct view *view_repolist_setup(unsigned int w, unsigned int h, struct view *applist)
{

    view_init(&view, show, hide, render, keydown);
    menu_init(&menu, menuitems, 3);
    menu_inititem(&menuitems[0], "All", MENUITEM_FLAG_NORMAL);
    menu_inititem(&menuitems[1], "Installed", MENUITEM_FLAG_NORMAL);
    menu_inititem(&menuitems[2], "Official", MENUITEM_FLAG_NORMAL);
    menu_setrow(&menu, 0);
    box_init(&menu.box, 0, 0, w, h);

    applistview = applist;

    return &view;

}

