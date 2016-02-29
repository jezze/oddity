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
    {{"Browse"}, MENUITEM_FLAG_NORMAL},
    {{"Settings"}, MENUITEM_FLAG_BLOCKED},
    {{"Exit"}, MENUITEM_FLAG_NORMAL}
};

static struct view view;
static struct view *categoryview;
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
    text_renderbox(&text, TEXT_COLOR_NORMAL);
    menu_render(&menu);

}

static void key(unsigned int keysym)
{

    switch (keysym)
    {

    case SDLK_LALT:
        ztore_setview(view.parent);

        break;

    case SDLK_UP:
        menu_prevrow(&menu);

        break;

    case SDLK_DOWN:
        menu_nextrow(&menu);

        break;

    case SDLK_LCTRL:
        switch (menu.currentitem)
        {

        case 0:
            ztore_setview(categoryview);

            break;

        case 2:
            ztore_quit();

            break;

        }

        break;

    }

}

struct view *view_frontsetup(unsigned int w, unsigned int h, struct view *category)
{

    view_init(&view, init, destroy, render, key);

    categoryview = category;
    text.text.content = "Welcome to Ztore!";
    menu.items = menuitems;
    menu.total = 3;

    box_init(&text.box, 0, 0, w, (4 * RENDER_ROWHEIGHT) + (2 * RENDER_PADDING));
    box_init(&menu.box, 0, h - (menu.total * RENDER_ROWHEIGHT) - (2 * RENDER_PADDING), w, (menu.total * RENDER_ROWHEIGHT) + (2 * RENDER_PADDING));
    menu_setrow(&menu, 0);

    return &view;

}

