#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "ztore.h"
#include "text.h"
#include "menu.h"
#include "render.h"

static struct menuitem menuitems[32] = {
    {{"Install"}, 1, MENUITEM_FLAG_NORMAL},
    {{"Remove"}, 2, MENUITEM_FLAG_NORMAL}
};

static struct textbox text = {{"Game title here."}, {0 + MENU_PADDING, 0 + MENU_PADDING, 320 - MENU_PADDING * 2, 96}};
static struct menu menu = {menuitems, 2, 0, {0, 120, 320, 120}};

static void init()
{

    menu_setrow(&menu, 0);

}

static void render()
{

    render_background();
    render_textbox(&text);
    render_menu(&menu);

}

static void handlekey(unsigned int keysym)
{

    switch (keysym)
    {

    case SDLK_ESCAPE:
        view_set(2);

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

void view_showappsetup()
{

    view_register(3, init, 0, render, handlekey, handleevent);

}

