#include <stdlib.h>
#include <stdio.h>
#include "define.h"
#include "box.h"
#include "text.h"
#include "menu.h"
#include "file.h"
#include "db.h"
#include "view.h"
#include "view_uninstall.h"
#include "ztore.h"

static struct view view;
static struct db_app *app;
static struct box statusbox;
static struct menu menu;
static struct box menubox;
static struct menuitem menuitems[1];

static void place(unsigned int w, unsigned int h)
{

    box_setpartsize(&statusbox, w / 10, h / 10, 0, 0, 10, 8);
    box_setpartsize(&menubox, w / 10, h / 10, 0, 8, 10, 2);

}

static void renderconfirm(void)
{

    text_render(&statusbox, TEXT_COLOR_NORMAL, TEXT_ALIGN_LEFT, "Are you sure you want to uninstall?");
    menu_render(&menu, &menubox);

}

static void renderuninstalling(void)
{

    char progress[128];

    snprintf(progress, 128, "Uninstalling %s...", app->name);
    text_render(&statusbox, TEXT_COLOR_NORMAL, TEXT_ALIGN_LEFT, progress);

}

static void rendercomplete(void)
{

    text_render(&statusbox, TEXT_COLOR_NORMAL, TEXT_ALIGN_LEFT, "Uninstall complete!\n\nPress B to go back.");

}

static void renderfail(void)
{

    text_render(&statusbox, TEXT_COLOR_NORMAL, TEXT_ALIGN_LEFT, "Uninstall failed!\n\nPress B to go back.");

}

static void buttonoff(unsigned int key)
{

}

static void buttonback(unsigned int key)
{

    switch (key)
    {

    case KEY_B:
        view_quit(&view);

        break;

    }

}

static void buttonconfirm(unsigned int key)
{

    menu_button(&menu, key);

    switch (key)
    {

    case KEY_B:
        view_quit(&view);

        break;

    }

}

static void updatestates(struct db_package *package)
{

    app->state = 0;

    db_saveappstate(app);

    package->state = 0;

    db_savepackagestate(package);

}

static unsigned int douninstall(struct db_packagelist *packagelist)
{

    unsigned int i;

    for (i = 0; i < packagelist->count; i++)
    {

        char path[128];

        updatestates(&packagelist->items[i]);
        file_getpackagepath(path, 128, packagelist->items[i].name);

        if (file_exist(path))
            file_removepackage(packagelist->items[i].name);

    }

    return 1;

}

static void uninstall(void)
{

    struct db_packagelist packagelist;

    ztore_setview(place, renderuninstalling, buttonoff);
    ztore_redraw();

    db_loadpackagesfromapp(&packagelist, app);

    if (douninstall(&packagelist))
        ztore_setview(place, rendercomplete, buttonback);
    else
        ztore_setview(place, renderfail, buttonback);

    ztore_redraw();
    db_freepackages(&packagelist);

}

static void load(void)
{

    ztore_setview(place, renderconfirm, buttonconfirm);

}

static void menu_onselect(void)
{

    switch (menu.currentitem)
    {

    case 0:
        uninstall();

        break;

    }

}

struct view *view_uninstall_setup(void)
{

    view_init(&view, load);
    box_init(&statusbox);
    box_init(&menubox);
    menu_init(&menu, menuitems, 1);
    menu_inititem(&menuitems[0], "Yes, I am sure", 0);
    menu_setrow(&menu, 0);

    menu.onselect = menu_onselect;

    return &view;

}

