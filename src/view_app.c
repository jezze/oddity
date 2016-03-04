#include <stdlib.h>
#include "define.h"
#include "box.h"
#include "text.h"
#include "menu.h"
#include "file.h"
#include "db.h"
#include "view.h"
#include "view_app.h"
#include "ztore.h"

static struct view_app view;
static struct textbox title;
static struct textbox shortdescription;
static struct menu menu;
static struct menuitem menuitems[2];

static void load()
{

    view.onload(&view.app);

    title.text.content = view.app.name;
    shortdescription.text.content = view.app.shortdescription;

    menu_setrow(&menu, 0);

}

static void unload()
{

    view.onunload(&view.app);

}

static void show()
{

    unload();
    load();
    ztore_flipview(&view.base);

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
        view_quit(&view.base);

        break;

    }

}

struct view_app *view_app_setup(unsigned int w, unsigned int h)
{

    view_init(&view.base, show, render, keydown);
    menu_init(&menu, menuitems, 2);
    menu_inititem(&menuitems[0], "Install", MENUITEM_FLAG_NORMAL);
    menu_inititem(&menuitems[1], "Uninstall", MENUITEM_FLAG_BLOCKED);
    menu_setrow(&menu, 0);
    box_init(&title.box, 0, 0, w, (1 * RENDER_ROWHEIGHT) + (2 * RENDER_PADDING));
    box_init(&shortdescription.box, 0, (1 * RENDER_ROWHEIGHT), w, (4 * RENDER_ROWHEIGHT) + (2 * RENDER_PADDING));
    box_init(&menu.box, 0, h - (menu.total * RENDER_ROWHEIGHT) - (2 * RENDER_PADDING), w, (menu.total * RENDER_ROWHEIGHT) + (2 * RENDER_PADDING));

    return &view;

}

