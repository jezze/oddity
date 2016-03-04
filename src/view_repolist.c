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
        if (!menu_isactive(&view.menu, view.menu.currentitem))
            break;

        switch (view.menu.currentitem)
        {

        case 0:
            ztore_flipview(&view.applistview->base);

            break;

        case 1:
            ztore_flipview(&view.applistview->base);

            break;

        case 2:
            ztore_flipview(&view.applistview->base);

            break;

        }

        break;

    case KEY_B:
        view_quit(&view.base);

        break;

    }

}

static void applistview_onquit()
{

    ztore_flipview(&view.base);

}

static unsigned int applistview_onload(struct db_applist *applist)
{

    if (applist->count)
        return 0;

    db_loadapps(applist);

    return 1;

}

struct view_repolist *view_repolist_setup(unsigned int w, unsigned int h)
{

    view_init(&view.base, show, render, keydown);
    menu_init(&view.menu, view.menuitems, 3);
    menu_inititem(&view.menuitems[0], "All");
    menu_inititem(&view.menuitems[1], "Installed");
    menu_inititem(&view.menuitems[2], "Official");
    menu_setrow(&view.menu, 0);
    box_init(&view.menu.box, 0, 0, w, h);

    view.applistview = view_applist_setup(w, h);
    view.applistview->onload = applistview_onload;
    view.applistview->base.onquit = applistview_onquit;

    return &view;

}

