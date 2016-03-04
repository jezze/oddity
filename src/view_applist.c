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
static struct view_app *appview;
static struct menu menu;
static struct textbox emptytextbox;
static unsigned int config_offset;

static void load()
{

    unsigned int i;

    db_countapps(&view.applist, "db/official.db");

    view.applist.items = malloc(sizeof (struct db_app) * view.applist.count);

    db_loadapps(view.applist.items, config_offset, view.applist.count, "db/official.db");

    menu.items = malloc(sizeof (struct menuitem) * view.applist.count);
    menu.total = view.applist.count;

    for (i = 0; i < menu.total; i++)
        menu_inititem(&menu.items[i], view.applist.items[i].name, MENUITEM_FLAG_NORMAL);

    menu_setrow(&menu, 0);

}

static void unload()
{

    unsigned int i;

    for (i = 0; i < view.applist.count; i++)
    {

        free(view.applist.items[i].name);
        free(view.applist.items[i].shortdescription);

    }

    free(view.applist.items);
    free(menu.items);

}

static void show()
{

    switch (view.base.state)
    {

    case VIEW_STATE_CONFIGURED:
        load();

        view.base.state = VIEW_STATE_DONE;

        break;

    case VIEW_STATE_RECONFIGURED:
        unload();
        load();

        view.base.state = VIEW_STATE_DONE;

        break;

    }

    ztore_flipview(&view.base);

}

static void render()
{

    if (view.applist.count)
        menu_render(&menu);
    else
        text_renderbox(&emptytextbox, TEXT_COLOR_NORMAL);

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

    case KEY_LEFT:
        menu_prevpage(&menu);

        break;

    case KEY_RIGHT:
        menu_nextpage(&menu);

        break;

    case KEY_A:
        view_app_config(view.applist.items[menu.currentitem].id);
        appview->base.show();

        break;

    case KEY_B:
        view_quit(&view.base);

        break;

    }

}

void view_applist_config(unsigned int offset)
{

    switch (view.base.state)
    {

    case VIEW_STATE_NONE:
        config_offset = offset;

        view.base.state = VIEW_STATE_CONFIGURED;

        break;

    case VIEW_STATE_DONE:
        if (config_offset == offset)
            break;

        config_offset = offset;

        view.base.state = VIEW_STATE_RECONFIGURED;

        break;

    }

}

struct view_applist *view_applist_setup(unsigned int w, unsigned int h)
{

    view_init(&view.base, show, render, keydown);
    text_init(&emptytextbox.text, "No items found.");
    box_init(&emptytextbox.box, 0, 0, w, (4 * RENDER_ROWHEIGHT) + (2 * RENDER_PADDING));
    menu_init(&menu, 0, 0);
    box_init(&menu.box, 0, 0, w, h);

    appview = view_app_setup(w, h);
    appview->base.onquit = show;

    return &view;

}

