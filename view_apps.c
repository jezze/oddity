#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "ztore.h"
#include "text.h"
#include "menu.h"
#include "render.h"

static struct menu appsmenu = {0, 0, 0, {0, 0, 320, 240}};

static void init()
{

    if (!appsmenu.items)
    {

        db_loadapps(&appsmenu, "db/apps.db");
        menu_setrow(&appsmenu, 0);

    }

}

static void render(SDL_Surface *display)
{

    render_menu(&appsmenu, display);

}

static void handlekey(unsigned int keysym)
{

    switch (keysym)
    {

    case SDLK_ESCAPE:
        view_set(0);

        break;

    case SDLK_UP:
        menu_prevrow(&appsmenu);

        break;

    case SDLK_DOWN:
        menu_nextrow(&appsmenu);

        break;

    case SDLK_LEFT:
        menu_prevpage(&appsmenu);

        break;

    case SDLK_RIGHT:
        menu_nextpage(&appsmenu);

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

