#include <stdlib.h>
#include "view.h"
#include "box.h"
#include "text.h"
#include "menu.h"
#include "backend.h"
#include "ztore.h"

static struct menuitem menuitems[3] = {
    {{"All"}, MENUITEM_FLAG_NORMAL},
    {{"Games"}, MENUITEM_FLAG_NORMAL},
    {{"Emulators"}, MENUITEM_FLAG_NORMAL}
};

static struct view view;
static struct view *browseview;
static struct menu menu;

static void show()
{

}

static void hide()
{

}

static void render()
{

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
            view_configbrowse(0);
            ztore_flipview(browseview);

            break;

        case 1:
            view_configbrowse(0);
            ztore_flipview(browseview);

            break;

        case 2:
            view_configbrowse(0);
            ztore_flipview(browseview);

            break;

        }

        break;

    case KEY_B:
        ztore_flipview(view.parent);

        break;

    }

}

struct view *view_categorysetup(unsigned int w, unsigned int h, struct view *browse)
{

    view_init(&view, show, hide, render, keydown);

    browseview = browse;
    menu.items = menuitems;
    menu.total = 3;

    menu_setrow(&menu, 0);
    box_init(&menu.box, 0, 0, w, h);

    return &view;

}

