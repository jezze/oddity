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

static void load()
{

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
        if (!menu_isactive(&view.menu, view.menu.currentitem))
            break;

        switch (view.menu.currentitem)
        {

        case 0:
            ztore_load(&view.repolistview->base);

            break;

        case 1:
            ztore_load(&view.syncview->base);

            break;

        case 3:
            ztore_quit();

            break;

        }

        break;

    }

}

static void repolistview_onquit()
{

    ztore_load(&view.base);

}

static void syncview_onquit()
{

    ztore_load(&view.base);

}

struct view_front *view_front_setup(unsigned int w, unsigned int h)
{

    view_init(&view.base, load, render, keydown);
    text_init(&view.text.text, "Welcome to Ztore!");
    box_init(&view.text.box, 0, 0, w, (4 * RENDER_ROWHEIGHT) + (2 * RENDER_PADDING));
    menu_init(&view.menu, view.menuitems, 4);
    menu_inititem(&view.menuitems[0], "Browse");
    menu_inititem(&view.menuitems[1], "Sync");
    menu_inititem(&view.menuitems[2], "Settings");
    menu_inititem(&view.menuitems[3], "Exit");
    menu_disable(&view.menu, 2);
    menu_setrow(&view.menu, 0);
    box_init(&view.menu.box, 0, h - (view.menu.total * RENDER_ROWHEIGHT) - (2 * RENDER_PADDING), w, (view.menu.total * RENDER_ROWHEIGHT) + (2 * RENDER_PADDING));

    view.repolistview = view_repolist_setup(w, h);
    view.repolistview->base.onquit = repolistview_onquit;
    view.syncview = view_sync_setup(w, h);
    view.syncview->base.onquit = syncview_onquit;

    return &view;

}

