#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "ztore.h"
#include "event.h"
#include "text.h"
#include "menu.h"
#include "render.h"

static struct view view;

static struct menuitem menuitems[4] = {
    {{"Apps"}, MENUITEM_FLAG_NORMAL},
    {{"Store"}, MENUITEM_FLAG_NORMAL},
    {{"Downloads"}, MENUITEM_FLAG_BLOCKED},
    {{"Exit"}, MENUITEM_FLAG_NORMAL}
};

static char *greeting = "Hello and welcome!\n\nThis is a very long text that I am using to see if my wordwrap is working properly.";
static struct textbox text;
static struct menu menu;

static void view_oninit()
{

}

static void view_ondestroy()
{

}

static void view_onrender()
{

    render_background();
    render_textbox(&text, 160, 192, 192);
    menu_render(&menu);

}

static void view_onkey(unsigned int keysym)
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
            event_showview(1);

            break;

        case 1:
            event_showview(2);

            break;

        case 3:
            event_quit();

            break;

        }

        break;

    }

}

static void onevent(unsigned int type, void *data)
{

    struct event_exitview *exitview;
    struct event_showview *showview;

    switch (type)
    {

    case EVENT_TYPE_EXITVIEW:
        exitview = data;

        switch (exitview->id)
        {

        case 1:
        case 2:
            view_set(&view);

            break;

        }

        break;

    case EVENT_TYPE_SHOWVIEW:
        showview = data;

        if (showview->id == 0)
            view_set(&view);

        break;

    }

}

void view_frontsetup(unsigned int w, unsigned int h)
{

    event_register(onevent);
    view_init(&view, view_oninit, view_ondestroy, view_onrender, view_onkey);
    view_set(&view);

    menu.items = menuitems;
    menu.total = 4;
    menu.box.w = w;
    menu.box.h = (menu.total * RENDER_ROWHEIGHT) + (2 * RENDER_PADDING);
    menu.box.x = 0;
    menu.box.y = h - menu.box.h;

    menu_setrow(&menu, 0);

    text.text.content = greeting;
    text.box.w = w;
    text.box.h = (4 * RENDER_ROWHEIGHT) + (2 * RENDER_PADDING);
    text.box.x = 0;
    text.box.y = 0;

}

