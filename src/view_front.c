#include <stdlib.h>
#include "define.h"
#include "box.h"
#include "text.h"
#include "menu.h"
#include "view.h"
#include "view_repolist.h"
#include "view_sync.h"
#include "view_front.h"
#include "ztore.h"

static struct view_front view;

static void show()
{

    ztore_flipview(&view.base);

}

static void render()
{

    text_renderbox(&view.text, TEXT_COLOR_NORMAL);
    menu_render(&view.menu);

}

static void keydown(unsigned int key)
{

    switch (key)
    {

    case KEY_UP:
        menu_prevrow(&view.menu);

        break;

    case KEY_DOWN:
        menu_nextrow(&view.menu);

        break;

    case KEY_A:
        switch (view.menu.currentitem)
        {

        case 0:
            view.repolistview->base.show();

            break;

        case 1:
            view.syncview->base.show();

            break;

        case 3:
            ztore_quit();

            break;

        }

        break;

    }

}

struct view_front *view_front_setup(unsigned int w, unsigned int h)
{

    view_init(&view.base, show, render, keydown);
    text_init(&view.text.text, "Welcome to Ztore!");
    box_init(&view.text.box, 0, 0, w, (4 * RENDER_ROWHEIGHT) + (2 * RENDER_PADDING));
    menu_init(&view.menu, view.menuitems, 4);
    menu_inititem(&view.menuitems[0], "Browse", MENUITEM_FLAG_NORMAL);
    menu_inititem(&view.menuitems[1], "Sync", MENUITEM_FLAG_NORMAL);
    menu_inititem(&view.menuitems[2], "Settings", MENUITEM_FLAG_BLOCKED);
    menu_inititem(&view.menuitems[3], "Exit", MENUITEM_FLAG_NORMAL);
    menu_setrow(&view.menu, 0);
    box_init(&view.menu.box, 0, h - (view.menu.total * RENDER_ROWHEIGHT) - (2 * RENDER_PADDING), w, (view.menu.total * RENDER_ROWHEIGHT) + (2 * RENDER_PADDING));

    view.repolistview = view_repolist_setup(w, h);
    view.repolistview->base.onquit = show;
    view.syncview = view_sync_setup(w, h);
    view.syncview->base.onquit = show;

    return &view;

}

