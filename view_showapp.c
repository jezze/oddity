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

static void init()
{

    db_loadapp(&app, 12, "db/official.db");    

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
    text_renderbox(&title, 224, 224, 224);
    text_renderbox(&shortdescription, 160, 192, 192);
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

static void load(unsigned int id)
{

}

struct view *view_showappsetup(unsigned int w, unsigned int h)
{

    view_init(&view, init, destroy, render, key, load);

    menu.items = menuitems;
    menu.total = 2;

    box_init(&title.box, 0, 0, w, (1 * RENDER_ROWHEIGHT) + (2 * RENDER_PADDING));
    box_init(&shortdescription.box, 0, (1 * RENDER_ROWHEIGHT) + (2 * RENDER_PADDING), w, (4 * RENDER_ROWHEIGHT) + (2 * RENDER_PADDING));
    box_init(&menu.box, 0, h - (menu.total * RENDER_ROWHEIGHT) - (2 * RENDER_PADDING), w, (menu.total * RENDER_ROWHEIGHT) + (2 * RENDER_PADDING));
    menu_setrow(&menu, 0);

    return &view;

}

