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

static void init()
{

}

static void destroy()
{

}

static void render()
{

    menu_render(&menu);

}

static void key(unsigned int keysym)
{

    switch (keysym)
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
            ztore_setview(browseview);

            break;

        case 1:
            ztore_setview(browseview);

            break;

        case 2:
            ztore_setview(browseview);

            break;

        }

        break;

    case KEY_B:
        ztore_setview(view.parent);

        break;

    }

}

struct view *view_categorysetup(unsigned int w, unsigned int h, struct view *browse)
{

    view_init(&view, init, destroy, render, key);

    browseview = browse;
    menu.items = menuitems;
    menu.total = 3;

    box_init(&menu.box, 0, 0, w, h);
    menu_setrow(&menu, 0);

    return &view;

}

