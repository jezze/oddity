#include <stdlib.h>
#include "view.h"
#include "box.h"
#include "text.h"
#include "menu.h"
#include "db.h"
#include "file.h"
#include "backend.h"
#include "ztore.h"

static struct menuitem menuitems[2] = {
    {{"Install"}, MENUITEM_FLAG_NORMAL},
    {{"Remove"}, MENUITEM_FLAG_NORMAL}
};

static struct view view;
static struct db_app app;
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

    text_renderbox(&title, TEXT_COLOR_TITLE);
    text_renderbox(&shortdescription, TEXT_COLOR_NORMAL);
    menu_render(&menu);

}

static void keydown(unsigned int key)
{

    switch (key)
    {

    case KEY_UP:
        menu_prevrow(&menu);

        break;

    case KEY_DOWN:
        menu_nextrow(&menu);

        break;

    case KEY_A:
        switch (menu.currentitem)
        {

        case 0:
            if (file_matchsha1("griffon.opk", "9a45660a3aad10bdedb77a676dc197637f2be1c2"))
                menu_nextrow(&menu);

            break;

        }

        break;

    case KEY_B:
        ztore_setview(view.parent);

        break;

    }

}

void view_loadshowapp(unsigned int id)
{

    dataid = id;

}

struct view *view_showappsetup(unsigned int w, unsigned int h)
{

    view_init(&view, init, destroy, render, keydown);

    menu.items = menuitems;
    menu.total = 2;

    box_init(&title.box, 0, 0, w, (1 * RENDER_ROWHEIGHT) + (2 * RENDER_PADDING));
    box_init(&shortdescription.box, 0, (1 * RENDER_ROWHEIGHT), w, (4 * RENDER_ROWHEIGHT) + (2 * RENDER_PADDING));
    box_init(&menu.box, 0, h - (menu.total * RENDER_ROWHEIGHT) - (2 * RENDER_PADDING), w, (menu.total * RENDER_ROWHEIGHT) + (2 * RENDER_PADDING));
    menu_setrow(&menu, 0);

    return &view;

}

