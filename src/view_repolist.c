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

static void show()
{

    ztore_flipview(&view.base);

}

static void render()
{

    menu_render(&view.menu);

}

static void keydown(unsigned int key)
{

    switch (key)
    {

    case KEY_UP:
        menu_prevrow(&view.menu);

        break;

    case KEY_DOWN:
        menu_nextrow(&view.menu);

        break;

    case KEY_A:
        switch (view.menu.currentitem)
        {

        case 0:
            view.applistview->base.show();

            break;

        case 1:
            view.applistview->base.show();

            break;

        case 2:
            view.applistview->base.show();

            break;

        }

        break;

    case KEY_B:
        view_quit(&view.base);

        break;

    }

}

static unsigned int applistview_onload(struct db_applist *applist)
{

    if (applist->count)
        return 0;

    db_countapps(applist);

    applist->items = malloc(sizeof (struct db_app) * applist->count);

    db_loadapps(applist->items, 0, applist->count);

    return 1;

}

static unsigned int applistview_onunload(struct db_applist *applist)
{

    return 0;

}

struct view_repolist *view_repolist_setup(unsigned int w, unsigned int h)
{

    view_init(&view.base, show, render, keydown);
    menu_init(&view.menu, view.menuitems, 3);
    menu_inititem(&view.menuitems[0], "All", MENUITEM_FLAG_NORMAL);
    menu_inititem(&view.menuitems[1], "Installed", MENUITEM_FLAG_NORMAL);
    menu_inititem(&view.menuitems[2], "Official", MENUITEM_FLAG_NORMAL);
    menu_setrow(&view.menu, 0);
    box_init(&view.menu.box, 0, 0, w, h);

    view.applistview = view_applist_setup(w, h);
    view.applistview->onload = applistview_onload;
    view.applistview->onunload = applistview_onunload;
    view.applistview->base.onquit = show;

    return &view;

}

