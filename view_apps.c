#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "view.h"
#include "app.h"
#include "box.h"
#include "text.h"
#include "menu.h"
#include "db.h"
#include "render.h"

static struct view view;
static struct applist applist;
static struct menu menu;

static void init()
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

static void destroy()
{

}

static void render()
{

    render_background();
    menu_render(&menu);

}

static void key(unsigned int keysym)
{

    switch (keysym)
    {

    case SDLK_ESCAPE:
        view_set(view.parent);

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

struct view *view_appssetup(unsigned int w, unsigned int h)
{

    view_init(&view, init, destroy, render, key);
    box_init(&menu.box, 0, 0, w, h);

    return &view;

}

