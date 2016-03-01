#include <stdlib.h>
#include "view.h"
#include "app.h"
#include "box.h"
#include "text.h"
#include "menu.h"
#include "db.h"
#include "backend.h"
#include "ztore.h"

static struct view view;
static struct view *showappview;
static struct applist applist;
static struct menu menu;
static struct textbox emptytextbox;

static void init()
{

    if (!menu.total)
    {

        unsigned int i;

        applist.count = db_countapps("db/official.db");
        applist.items = malloc(sizeof (struct app) * applist.count);

        db_loadapps(applist.items, 0, applist.count, "db/official.db");

        menu.total = applist.count;
        menu.items = malloc(sizeof (struct menuitem) * menu.total);

        for (i = 0; i < menu.total; i++)
        {

            menu.items[i].type = MENUITEM_FLAG_NORMAL;
            menu.items[i].text.content = applist.items[i].name;

        }

        menu_setrow(&menu, 0);

    }

}

static void destroy()
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
        view_loadshowapp(applist.items[menu.currentitem].id);
        ztore_setview(showappview);

        break;

    case KEY_B:
        ztore_setview(view.parent);

        break;

    }

}

struct view *view_browsesetup(unsigned int w, unsigned int h, struct view *showapp)
{

    view_init(&view, init, destroy, render, keydown);

    showappview = showapp;
    emptytextbox.text.content = "No apps have been installed.";

    box_init(&emptytextbox.box, 0, 0, w, (4 * RENDER_ROWHEIGHT) + (2 * RENDER_PADDING));
    box_init(&menu.box, 0, 0, w, h);

    return &view;

}

