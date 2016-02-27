#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "ztore.h"
#include "app.h"
#include "text.h"
#include "menu.h"
#include "db.h"
#include "render.h"

static struct menuitem menuitems[32] = {
    {{"Install"}, 1, MENUITEM_FLAG_NORMAL},
    {{"Remove"}, 2, MENUITEM_FLAG_NORMAL}
};

static struct app app;
static struct textbox title = {{"Invalid entry"}, {0 + MENU_PADDING, 0 + MENU_PADDING, 320 - MENU_PADDING * 2, 96}};
static struct menu menu = {menuitems, 2, 0, {0, 120, 320, 120}};

static void init(unsigned int from, unsigned int id)
{

    db_loadapp(&app, id, "db/official.db");    
    menu_setrow(&menu, 0);

    title.text.content = app.name;

}

static void destroy()
{

    free(app.name);

}

static void render()
{

    render_background();
    render_textbox(&title);
    render_menu(&menu);

}

static void handlekey(unsigned int keysym)
{

    switch (keysym)
    {

    case SDLK_ESCAPE:
        view_set(2, 3, 0);

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

    view_register(3, init, destroy, render, handlekey, handleevent);

}

