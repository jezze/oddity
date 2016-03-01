#include <stdlib.h>
#include "view.h"
#include "box.h"
#include "text.h"
#include "menu.h"
#include "db.h"
#include "backend.h"
#include "ztore.h"

static struct view view;
static struct view *showappview;
static struct db_applist applist;
static struct menu menu;
static struct textbox emptytextbox;
static unsigned int config_offset;

static void load()
{

    unsigned int i;

    db_countapps(&applist, "db/official.db");

    applist.items = malloc(sizeof (struct db_app) * applist.count);

    db_loadapps(applist.items, config_offset, applist.count, "db/official.db");
    menu_init(&menu, malloc(sizeof (struct menuitem) * applist.count), applist.count);

    for (i = 0; i < menu.total; i++)
        menu_inititem(&menu.items[i], applist.items[i].name, MENUITEM_FLAG_NORMAL);

    menu_setrow(&menu, 0);

}

static void unload()
{

    unsigned int i;

    for (i = 0; i < applist.count; i++)
    {

        free(applist.items[i].name);
        free(applist.items[i].shortdescription);

    }

    free(applist.items);
    free(menu.items);

}

static void show()
{

}

static void hide()
{

}

static void render()
{

    if (applist.count)
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
        view_configshowapp(applist.items[menu.currentitem].id);
        ztore_flipview(showappview);

        break;

    case KEY_B:
        ztore_flipview(view.parent);

        break;

    }

}

void view_configbrowse(unsigned int offset)
{

    if (view.state == VIEW_STATE_LOADED && config_offset != offset)
    {

        unload();

        config_offset = offset;

        load();

    }

    else if (view.state == VIEW_STATE_NONE)
    {

        config_offset = offset;

        load();

        view.state = VIEW_STATE_LOADED;

    }

}

struct view *view_browsesetup(unsigned int w, unsigned int h, struct view *showapp)
{

    view_init(&view, show, hide, render, keydown);

    text_init(&emptytextbox.text, "No items found.");
    box_init(&emptytextbox.box, 0, 0, w, (4 * RENDER_ROWHEIGHT) + (2 * RENDER_PADDING));
    menu_init(&menu, 0, 0);
    box_init(&menu.box, 0, 0, w, h);

    showappview = showapp;

    return &view;

}

