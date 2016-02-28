#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "ztore.h"
#include "event.h"
#include "app.h"
#include "text.h"
#include "menu.h"
#include "db.h"
#include "render.h"

static struct view view;

static struct menuitem menuitems[2] = {
    {{"Install"}, MENUITEM_FLAG_NORMAL},
    {{"Remove"}, MENUITEM_FLAG_NORMAL}
};

static struct app app;
static struct textbox title;
static struct textbox shortdescription;
static struct menu menu;

static void view_oninit()
{

    db_loadapp(&app, 12, "db/official.db");    

    title.text.content = app.name;
    shortdescription.text.content = app.shortdescription;

}

static void view_ondestroy()
{

    free(app.name);
    free(app.shortdescription);

}

static void view_onrender()
{

    render_background();
    render_textbox(&title, 224, 224, 224);
    render_textbox(&shortdescription, 160, 192, 192);
    menu_render(&menu);

}

static void view_onkey(unsigned int keysym)
{

    switch (keysym)
    {

    case SDLK_ESCAPE:
        event_exitview(3);

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

static void onevent(unsigned int type, void *data)
{

    struct event_showview *showview;

    switch (type)
    {

    case EVENT_TYPE_SHOWVIEW:
        showview = data;

        if (showview->id == 3)
            view_set(&view);

        break;

    }

}

void view_showappsetup(unsigned int w, unsigned int h)
{

    event_register(onevent);
    view_init(&view, view_oninit, view_ondestroy, view_onrender, view_onkey);

    menu.items = menuitems;
    menu.total = 2;
    menu.box.w = w;
    menu.box.h = (menu.total * RENDER_ROWHEIGHT) + (2 * RENDER_PADDING);
    menu.box.x = 0;
    menu.box.y = h - menu.box.h;

    menu_setrow(&menu, 0);

    title.box.w = w;
    title.box.h = (1 * RENDER_ROWHEIGHT) + (2 * RENDER_PADDING);
    title.box.x = 0;
    title.box.y = 0;

    shortdescription.box.w = w;
    shortdescription.box.h = (4 * RENDER_ROWHEIGHT) + (2 * RENDER_PADDING);
    shortdescription.box.x = 0;
    shortdescription.box.y = title.box.h;

}

