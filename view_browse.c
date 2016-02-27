#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "ztore.h"
#include "text.h"
#include "menu.h"
#include "render.h"

static struct menu menu = {0, 0, 0, {0, 0, 320, 240}};

static void init()
{

    if (!menu.items)
    {

        db_loadapps(&menu, "db/official.db");
        menu_setrow(&menu, 0);

    }

}

static void render()
{

    render_background();
    render_menu(&menu);

}

static void handlekey(unsigned int keysym)
{

    switch (keysym)
    {

    case SDLK_ESCAPE:
        view_set(0);

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
        view_handleevent(2, menu.items[menu.currentitem].id);

        break;

    }

}

static void handleevent(unsigned int id)
{

    view_set(3);

}

void view_browsesetup()
{

    view_register(2, init, 0, render, handlekey, handleevent);

}

