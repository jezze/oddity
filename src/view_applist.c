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

static void render()
{

    if (view.applist.count)
        menu_render(&view.menu, &view.menubox);
    else
        text_render(&view.emptytextbox, TEXT_COLOR_NORMAL, TEXT_ALIGN_LEFT, "No items found.");

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

    unsigned int i;
    char *stateinfo[] = {0, "New", "Updated", "Installed"};

    view.onload();

    free(view.menu.items);

    view.menu.items = malloc(sizeof (struct menuitem) * view.applist.count);
    view.menu.total = view.applist.count;

    for (i = 0; i < view.menu.total; i++)
        menu_inititem(&view.menu.items[i], view.applist.items[i].name, stateinfo[view.applist.items[i].state]);

    menu_setrow(&view.menu, 0);

}

static void menu_onselect()
{

    if (view.applist.count)
        view_load(&view.appview->base);

}

static void appview_onquit()
{

    view_load(&view.base);

}

static void appview_onload()
{

    view.appview->app = &view.applist.items[view.menu.currentitem];

}

struct view_applist *view_applist_setup(unsigned int w, unsigned int h)
{

    view_init(&view.base, load, render, keydown);
    box_init(&view.emptytextbox, 0, 0, w, (1 * RENDER_ROWHEIGHT) + (2 * RENDER_PADDING));
    box_init(&view.menubox, 0, 0, w, h);
    menu_init(&view.menu, 0, 0);

    view.menu.onselect = menu_onselect;
    view.appview = view_app_setup(w, h);
    view.appview->onload = appview_onload;
    view.appview->base.onquit = appview_onquit;

    return &view;

}

