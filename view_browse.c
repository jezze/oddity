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
static struct menu menu = {0, 0, 0, {0, 0, 320, 240}};

static void view_oninit(unsigned int id)
{

    if (!menu.items)
    {

        db_loadapps(&menu, "db/official.db");
        menu_setrow(&menu, 0);

    }

}

static void view_ondestroy()
{

}


static void view_onrender()
{

    render_background();
    render_menu(&menu);

}

static void view_onkey(unsigned int keysym)
{

    switch (keysym)
    {

    case SDLK_ESCAPE:
        event_exitview(2);

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

    case SDLK_RETURN:
        break;

    }

}

static void onevent(unsigned int type, void *data)
{

    struct event_showview *exitview;
    struct event_showview *showview;

    switch (type)
    {

    case EVENT_TYPE_EXITVIEW:
        exitview = data;

        switch (exitview->id)
        {

        case 3:
            view_set(&view, 0);

            break;

        }

        break;


    case EVENT_TYPE_SHOWVIEW:
        showview = data;

        if (showview->id == 2)
            view_set(&view, 0);

        break;

    }

}

void view_browsesetup()
{

    event_register(onevent);
    view_init(&view, view_oninit, view_ondestroy, view_onrender, view_onkey);

}

