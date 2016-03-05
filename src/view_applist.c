#include <stdlib.h>
#include "define.h"
#include "box.h"
#include "text.h"
#include "menu.h"
#include "db.h"
#include "view.h"
#include "view_app.h"
#include "view_applist.h"
#include "ztore.h"

static struct view_applist view;

static void load()
{

    unsigned int i;

    view.onload();

    free(view.menu.items);

    view.menu.items = malloc(sizeof (struct menuitem) * view.applist.count);
    view.menu.total = view.applist.count;

    for (i = 0; i < view.menu.total; i++)
        menu_inititem(&view.menu.items[i], view.applist.items[i].name);

    menu_setrow(&view.menu, 0);

}

static void render()
{

    if (view.applist.count)
        menu_render(&view.menu);
    else
        text_renderbox(&view.emptytextbox, TEXT_COLOR_NORMAL);

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

    case KEY_LEFT:
        menu_prevpage(&view.menu);

        break;

    case KEY_RIGHT:
        menu_nextpage(&view.menu);

        break;

    case KEY_A:
        if (!menu_isactive(&view.menu, view.menu.currentitem))
            break;

        ztore_load(&view.appview->base);

        break;

    case KEY_B:
        view_quit(&view.base);

        break;

    }

}

static void appview_onquit()
{

    ztore_load(&view.base);

}

static void appview_onload()
{

    view.appview->app = &view.applist.items[view.menu.currentitem];

}

struct view_applist *view_applist_setup(unsigned int w, unsigned int h)
{

    view_init(&view.base, load, render, keydown);
    text_init(&view.emptytextbox.text, "No items found.");
    box_init(&view.emptytextbox.box, 0, 0, w, (4 * RENDER_ROWHEIGHT) + (2 * RENDER_PADDING));
    menu_init(&view.menu, 0, 0);
    box_init(&view.menu.box, 0, 0, w, h);

    view.appview = view_app_setup(w, h);
    view.appview->onload = appview_onload;
    view.appview->base.onquit = appview_onquit;

    return &view;

}

