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

static struct view view;
static struct view *repolistview;
static struct view *syncview;
static struct box greetingbox;
static struct menu menu;
static struct box menubox;
static struct menuitem menuitems[4];

static void render(void)
{

    text_render(&greetingbox, TEXT_COLOR_NORMAL, TEXT_ALIGN_LEFT, "Welcome to Ztore!");
    menu_render(&menu, &menubox);

}

static void keydown(unsigned int key)
{

    menu_keydown(&menu, key);

}

static void load(void)
{

}

static void menu_onselect(void)
{

    switch (menu.currentitem)
    {

    case 0:
        view_load(repolistview);

        break;

    case 1:
        view_load(syncview);

        break;

    case 2:
        break;

    case 3:
        view_quitloop();

        break;

    }

}

static void repolistview_onquit(void)
{

    view_load(&view);

}

static void syncview_onquit(void)
{

    view_load(&view);

}

struct view *view_front_setup(unsigned int w, unsigned int h)
{

    view_init(&view, load, render, keydown);
    box_init(&greetingbox);
    box_init(&menubox);
    box_setpartsize(&greetingbox, w / 10, h / 10, 0, 0, 10, 5);
    box_setpartsize(&menubox, w / 10, h / 10, 0, 5, 10, 5);
    menu_init(&menu, menuitems, 4);
    menu_inititem(&menuitems[0], "Browse", 0);
    menu_inititem(&menuitems[1], "Sync", 0);
    menu_inititem(&menuitems[2], "Settings", 0);
    menu_inititem(&menuitems[3], "Exit", 0);
    menu_disable(&menu, 2);
    menu_setrow(&menu, 0);

    menu.onselect = menu_onselect;
    repolistview = view_repolist_setup(w, h);
    repolistview->onquit = repolistview_onquit;
    syncview = view_sync_setup(w, h);
    syncview->onquit = syncview_onquit;

    return &view;

}

