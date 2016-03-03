#include <stdlib.h>
#include "view.h"
#include "box.h"
#include "text.h"
#include "menu.h"
#include "backend.h"
#include "ztore.h"

static struct view view;
static struct view *repolistview;
static struct view *syncview;
static struct textbox text;
static struct menu menu;
static struct menuitem menuitems[4];

static void show()
{

    ztore_flipview(&view);

}

static void render()
{

    text_renderbox(&text, TEXT_COLOR_NORMAL);
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
            repolistview->show();

            break;

        case 1:
            syncview->show();

            break;

        case 3:
            ztore_quit();

            break;

        }

        break;

    }

}

struct view *view_front_setup(unsigned int w, unsigned int h, struct view *repolist, struct view *sync)
{

    view_init(&view, show, render, keydown);
    text_init(&text.text, "Welcome to Ztore!");
    box_init(&text.box, 0, 0, w, (4 * RENDER_ROWHEIGHT) + (2 * RENDER_PADDING));
    menu_init(&menu, menuitems, 4);
    menu_inititem(&menuitems[0], "Browse", MENUITEM_FLAG_NORMAL);
    menu_inititem(&menuitems[1], "Sync", MENUITEM_FLAG_NORMAL);
    menu_inititem(&menuitems[2], "Settings", MENUITEM_FLAG_BLOCKED);
    menu_inititem(&menuitems[3], "Exit", MENUITEM_FLAG_NORMAL);
    menu_setrow(&menu, 0);
    box_init(&menu.box, 0, h - (menu.total * RENDER_ROWHEIGHT) - (2 * RENDER_PADDING), w, (menu.total * RENDER_ROWHEIGHT) + (2 * RENDER_PADDING));

    repolistview = repolist;
    repolistview->onquit = show;
    syncview = sync;
    syncview->onquit = show;

    return &view;

}

