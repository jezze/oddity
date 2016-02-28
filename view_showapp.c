#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "view.h"
#include "event.h"
#include "app.h"
#include "box.h"
#include "text.h"
#include "menu.h"
#include "db.h"
#include "render.h"

static struct menuitem menuitems[2] = {
    {{"Install"}, MENUITEM_FLAG_NORMAL},
    {{"Remove"}, MENUITEM_FLAG_NORMAL}
};

static struct view view;
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
    text_renderbox(&title, 224, 224, 224);
    text_renderbox(&shortdescription, 160, 192, 192);
    menu_render(&menu);

}

static void view_onkey(unsigned int keysym)
{

    switch (keysym)
    {

    case SDLK_ESCAPE:
        event_exitview(&view, 3);

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

    box_init(&title.box, 0, 0, w, (1 * RENDER_ROWHEIGHT) + (2 * RENDER_PADDING));
    box_init(&shortdescription.box, 0, (1 * RENDER_ROWHEIGHT) + (2 * RENDER_PADDING), w, (4 * RENDER_ROWHEIGHT) + (2 * RENDER_PADDING));
    box_init(&menu.box, 0, h - (menu.total * RENDER_ROWHEIGHT) - (2 * RENDER_PADDING), w, (menu.total * RENDER_ROWHEIGHT) + (2 * RENDER_PADDING));
    menu_setrow(&menu, 0);

}

