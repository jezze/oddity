#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "ztore.h"
#include "text.h"
#include "menu.h"
#include "render.h"

static struct menuitem menuitems[32] = {
    {{"Apps"}, 1, MENUITEM_FLAG_NORMAL},
    {{"Store"}, 2, MENUITEM_FLAG_NORMAL},
    {{"Downloads"}, 0, MENUITEM_FLAG_BLOCKED},
    {{"Exit"}, 8, MENUITEM_FLAG_NORMAL}
};

static struct textbox fronttext = {{"Hello and welcome!\n\nThis is a very long text that I am using to see if my wordwrap is working properly."}, {0 + MENU_PADDING, 0 + MENU_PADDING, 320 - MENU_PADDING * 2, 96}};
static struct menu frontmenu = {menuitems, 4, 0, {0, 120, 320, 120}};

static void init()
{

    menu_setrow(&frontmenu, 0);

}

static void render()
{

    render_background();
    render_textbox(&fronttext);
    render_menu(&frontmenu);

}

static void handlekey(unsigned int keysym)
{

    switch (keysym)
    {

    case SDLK_UP:
        menu_prevrow(&frontmenu);

        break;

    case SDLK_DOWN:
        menu_nextrow(&frontmenu);

        break;

    case SDLK_LEFT:
        menu_prevpage(&frontmenu);

        break;

    case SDLK_RIGHT:
        menu_nextpage(&frontmenu);

        break;

    case SDLK_RETURN:
        view_handleevent(0, frontmenu.items[frontmenu.currentitem].id);

        break;

    }

}

static void handleevent(unsigned int id)
{

    switch (id)
    {

    case 1:
        view_set(1);

        break;

    case 2:
        view_set(2);

        break;

    case 8:
        ztore_changestate(0);

        break;

    }

}

void view_frontsetup()
{

    view_register(0, init, 0, render, handlekey, handleevent);

}

