#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "view.h"
#include "box.h"
#include "text.h"
#include "menu.h"
#include "render.h"
#include "ztore.h"

static struct menuitem menuitems[4] = {
    {{"Apps"}, MENUITEM_FLAG_NORMAL},
    {{"Store"}, MENUITEM_FLAG_NORMAL},
    {{"Downloads"}, MENUITEM_FLAG_BLOCKED},
    {{"Exit"}, MENUITEM_FLAG_NORMAL}
};

static char *greeting = "Hello and welcome!\n\nThis is a very long text that I am using to see if my wordwrap is working properly.";
static struct view view;
static struct view *appsview;
static struct view *browseview;
static struct textbox text;
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
    text_renderbox(&text, 160, 192, 192);
    menu_render(&menu);

}

static void key(unsigned int keysym)
{

    switch (keysym)
    {

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
            ztore_setview(appsview);

            break;

        case 1:
            ztore_setview(browseview);

            break;

        case 3:
            ztore_quit();

            break;

        }

        break;

    }

}

struct view *view_frontsetup(unsigned int w, unsigned int h, struct view *apps, struct view *browse)
{

    view_init(&view, init, destroy, render, key);

    appsview = apps;
    browseview = browse;
    text.text.content = greeting;
    menu.items = menuitems;
    menu.total = 4;

    box_init(&text.box, 0, 0, w, (4 * RENDER_ROWHEIGHT) + (2 * RENDER_PADDING));
    box_init(&menu.box, 0, h - (menu.total * RENDER_ROWHEIGHT) - (2 * RENDER_PADDING), w, (menu.total * RENDER_ROWHEIGHT) + (2 * RENDER_PADDING));
    menu_setrow(&menu, 0);

    return &view;

}

