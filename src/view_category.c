#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "view.h"
#include "box.h"
#include "text.h"
#include "menu.h"
#include "render.h"
#include "ztore.h"

static struct menuitem menuitems[3] = {
    {{"All"}, MENUITEM_FLAG_NORMAL},
    {{"Games"}, MENUITEM_FLAG_NORMAL},
    {{"Emulators"}, MENUITEM_FLAG_NORMAL}
};

static struct view view;
static struct view *browseview;
static struct menu menu;

static void init()
{

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
        ztore_setview(view.parent);

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

    case SDLK_RETURN:
        switch (menu.currentitem)
        {

        case 0:
            ztore_setview(browseview);

            break;

        case 1:
            ztore_setview(browseview);

            break;

        case 2:
            ztore_setview(browseview);

            break;

        }

        break;

    }

}

static void load(unsigned int id)
{

}

struct view *view_categorysetup(unsigned int w, unsigned int h, struct view *browse)
{

    view_init(&view, init, destroy, render, key, load);

    browseview = browse;
    menu.items = menuitems;
    menu.total = 3;

    box_init(&menu.box, 0, 0, w, h);
    menu_setrow(&menu, 0);

    return &view;

}

