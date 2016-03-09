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

static void render()
{

    text_render(&view.greetingbox, TEXT_COLOR_NORMAL, TEXT_ALIGN_LEFT, "Welcome to Ztore!");
    menu_render(&view.menu, &view.menubox);

}

static void keydown(unsigned int key)
{

    menu_keydown(&view.menu, key);

}

static void load()
{

}

static void menu_onselect()
{

    switch (view.menu.currentitem)
    {

    case 0:
        view_load(&view.repolistview->base);

        break;

    case 1:
        view_load(&view.syncview->base);

        break;

    case 2:
        break;

    case 3:
        view_quitloop();

        break;

    }

}

static void repolistview_onquit()
{

    view_load(&view.base);

}

static void syncview_onquit()
{

    view_load(&view.base);

}

struct view_front *view_front_setup(unsigned int w, unsigned int h)
{

    view_init(&view.base, load, render, keydown);
    box_init(&view.greetingbox, 0, 0, w, (4 * RENDER_ROWHEIGHT) + (2 * RENDER_PADDING));
    box_init(&view.menubox, 0, (4 * RENDER_ROWHEIGHT) + (2 * RENDER_PADDING), w, (4 * RENDER_ROWHEIGHT) + (2 * RENDER_PADDING));
    menu_init(&view.menu, view.menuitems, 4);
    menu_inititem(&view.menuitems[0], "Browse", 0);
    menu_inititem(&view.menuitems[1], "Sync", 0);
    menu_inititem(&view.menuitems[2], "Settings", 0);
    menu_inititem(&view.menuitems[3], "Exit", 0);
    menu_disable(&view.menu, 2);
    menu_setrow(&view.menu, 0);

    view.menu.onselect = menu_onselect;
    view.repolistview = view_repolist_setup(w, h);
    view.repolistview->base.onquit = repolistview_onquit;
    view.syncview = view_sync_setup(w, h);
    view.syncview->base.onquit = syncview_onquit;

    return &view;

}

