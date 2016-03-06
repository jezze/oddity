#include <stdlib.h>
#include "define.h"
#include "box.h"
#include "text.h"
#include "menu.h"
#include "file.h"
#include "db.h"
#include "view.h"
#include "view_uninstall.h"
#include "backend.h"
#include "ztore.h"

static struct view_uninstall view;

static void renderdefault()
{

    text_renderbox(&view.status, TEXT_COLOR_NORMAL, "Are you sure you want to uninstall?");
    menu_render(&view.menu);

}

static void renderworking()
{

    text_renderbox(&view.status, TEXT_COLOR_NORMAL, "Uninstalling...");

}

static void rendercomplete()
{

    text_renderbox(&view.status, TEXT_COLOR_NORMAL, "Uninstall complete!\n\nPress B to go back.");

}

static void renderfail()
{

    text_renderbox(&view.status, TEXT_COLOR_NORMAL, "Uninstall failed!\n\nPress B to go back.");

}

static void keydowndefault(unsigned int key)
{

    menu_keydown(&view.menu, key);

    switch (key)
    {

    case KEY_B:
        view_quit(&view.base);

        break;

    }

}

static void keydownworking(unsigned int key)
{

}

static void keydown(unsigned int key)
{

    switch (key)
    {

    case KEY_B:
        view_quit(&view.base);

        break;

    }

}

static void updatestates(struct db_package *package)
{

    view.app->state = 0;

    db_saveappstate(view.app);

    package->state = 0;

    db_savepackagestate(package);

}

static unsigned int checkpackageexist(struct db_package *package)
{

    char path[64];

    file_getpackagepath(path, 64, package->name);

    return file_exist(path) && file_matchsha1(path, package->sha1);

}

static unsigned int douninstall(struct db_packagelist *packagelist)
{

    unsigned int i;

    if (!packagelist->count)
        return 0;

    for (i = 0; i < packagelist->count; i++)
    {

        if (checkpackageexist(&packagelist->items[i]))
        {

            updatestates(&packagelist->items[i]);
            file_removepackage(packagelist->items[i].name);

            return 1;

        }

    }

    return 0;

}

static int uninstall(void *arg)
{

    struct db_packagelist packagelist;

    ztore_setmode(renderworking, keydownworking);
    db_loadpackagesfromapp(&packagelist, view.app);

    if (douninstall(&packagelist))
        ztore_setmode(rendercomplete, keydown);
    else
        ztore_setmode(renderfail, keydown);

    db_freepackages(&packagelist);

    return 0;

}

static void load()
{

    view.onload();

    ztore_setmode(renderdefault, keydowndefault);

}

static void confirm()
{

    void *uninstallthread = backend_createthread(uninstall, NULL);

    if (!uninstallthread)
        ztore_setmode(renderfail, keydown);

}

static void menu_onselect()
{

    switch (view.menu.currentitem)
    {

    case 0:
        confirm();

        break;

    }

}

struct view_uninstall *view_uninstall_setup(unsigned int w, unsigned int h)
{

    view_init(&view.base, load, renderdefault, keydowndefault);
    text_init(&view.status.text, 0);
    box_init(&view.status.box, 0, 0, w, (4 * RENDER_ROWHEIGHT) + (2 * RENDER_PADDING));
    menu_init(&view.menu, view.menuitems, 1);
    menu_inititem(&view.menuitems[0], "Yes, I am sure");
    menu_setrow(&view.menu, 0);
    box_init(&view.menu.box, 0, h - (view.menu.total * RENDER_ROWHEIGHT) - (2 * RENDER_PADDING), w, (view.menu.total * RENDER_ROWHEIGHT) + (2 * RENDER_PADDING));

    view.menu.onselect = menu_onselect;

    return &view;

}

