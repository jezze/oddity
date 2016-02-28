#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "view.h"
#include "event.h"
#include "app.h"
#include "box.h"
#include "text.h"
#include "menu.h"
#include "db.h"
#include "render.h"

static struct view view;
static struct applist applist;
static struct menu menu;

static void view_oninit()
{

    if (!menu.total)
    {

        unsigned int i;

        applist.count = db_countapps("db/apps.db");
        applist.items = malloc(sizeof (struct app) * applist.count);

        db_loadapps(applist.items, applist.count, "db/apps.db");

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

static void view_ondestroy()
{

}

static void view_onrender()
{

    render_background();
    menu_render(&menu);

}

static void view_onkey(unsigned int keysym)
{

    switch (keysym)
    {

    case SDLK_ESCAPE:
        event_exitview(&view, 1);

        break;

    case SDLK_UP:
        menu_prevrow(&menu);

        break;

    case SDLK_DOWN:
        menu_nextrow(&menu);

        break;

    case SDLK_LEFT:
        menu_prevpage(&menu);

        break;

    case SDLK_RIGHT:
        menu_nextpage(&menu);

        break;

    }

}

static void onevent(unsigned int type, void *data)
{

    struct event_showview *showview;

    switch (type)
    {

    case EVENT_TYPE_SHOWVIEW:
        showview = data;

        if (showview->id == 1)
            view_set(&view);

        break;

    }

}

void view_appssetup(unsigned int w, unsigned int h)
{

    event_register(onevent);
    view_init(&view, view_oninit, view_ondestroy, view_onrender, view_onkey);
    box_init(&menu.box, 0, 0, w, h);

}

