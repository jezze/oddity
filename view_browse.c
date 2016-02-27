#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "ztore.h"
#include "text.h"
#include "menu.h"
#include "render.h"

static struct menu browsemenu = {0, 0, 0, {0, 0, 320, 240}};

static void init()
{

    if (!browsemenu.items)
    {

        db_loadapps(&browsemenu, "db/official.db");
        menu_setrow(&browsemenu, 0);

    }

}

static void render()
{

    render_background();
    render_menu(&browsemenu);

}

static void handlekey(unsigned int keysym)
{

    switch (keysym)
    {

    case SDLK_ESCAPE:
        view_set(0);

        break;

    case SDLK_UP:
        menu_prevrow(&browsemenu);

        break;

    case SDLK_DOWN:
        menu_nextrow(&browsemenu);

        break;

    case SDLK_LEFT:
        menu_prevpage(&browsemenu);

        break;

    case SDLK_RIGHT:
        menu_nextpage(&browsemenu);

        break;

    }

}

static void handleevent(unsigned int id)
{

}

void view_browsesetup()
{

    view_register(2, init, 0, render, handlekey, handleevent);

}

