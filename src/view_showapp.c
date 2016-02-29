#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "view.h"
#include "app.h"
#include "box.h"
#include "text.h"
#include "menu.h"
#include "db.h"
#include "render.h"
#include "ztore.h"

static struct menuitem menuitems[2] = {
    {{"Install"}, MENUITEM_FLAG_NORMAL},
    {{"Remove"}, MENUITEM_FLAG_NORMAL}
};

static struct view view;
static struct app app;
static struct textbox title;
static struct textbox shortdescription;
static struct menu menu;
static unsigned int dataid;

static void init()
{

    db_loadapp(&app, dataid, "db/official.db");    

    title.text.content = app.name;
    shortdescription.text.content = app.shortdescription;

}

static void destroy()
{

    free(app.name);
    free(app.shortdescription);

}

static void render()
{

    render_background();
    text_renderbox(&title, TEXT_COLOR_TITLE);
    text_renderbox(&shortdescription, TEXT_COLOR_NORMAL);
    menu_render(&menu);

}

static void key(unsigned int keysym)
{

    switch (keysym)
    {

    case SDLK_ESCAPE:
        ztore_setview(view.parent);

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

void view_loadshowapp(unsigned int id)
{

    dataid = id;

}

struct view *view_showappsetup(unsigned int w, unsigned int h)
{

    view_init(&view, init, destroy, render, key);

    menu.items = menuitems;
    menu.total = 2;

    box_init(&title.box, 0, 0, w, (1 * RENDER_ROWHEIGHT) + (2 * RENDER_PADDING));
    box_init(&shortdescription.box, 0, (1 * RENDER_ROWHEIGHT), w, (4 * RENDER_ROWHEIGHT) + (2 * RENDER_PADDING));
    box_init(&menu.box, 0, h - (menu.total * RENDER_ROWHEIGHT) - (2 * RENDER_PADDING), w, (menu.total * RENDER_ROWHEIGHT) + (2 * RENDER_PADDING));
    menu_setrow(&menu, 0);

    return &view;

}

