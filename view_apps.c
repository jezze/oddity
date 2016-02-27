#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "ztore.h"
#include "text.h"
#include "menu.h"
#include "render.h"

static struct menu menu = {0, 0, 0, {0, 0, 320, 240}};

static void init(unsigned int from, unsigned int id)
{

    if (!menu.items)
    {

        db_loadapps(&menu, "db/apps.db");
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
        view_set(0, 1, 0);

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

static void handleevent(unsigned int id)
{

}

void view_appssetup()
{

    view_register(1, init, 0, render, handlekey, handleevent);

}

