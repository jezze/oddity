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
static char *status[4] = {
    "Are you sure you want to uninstall?",
    "Uninstalling...",
    "Uninstall complete!",
    "Uninstall failed!"
};

static void changestate(unsigned int state)
{

    view.state = state;

    ztore_redraw();

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

    changestate(1);
    db_loadpackagesfromapp(&packagelist, view.app);

    if (douninstall(&packagelist))
        changestate(2);
    else
        changestate(3);

    db_freepackages(&packagelist);

    return 0;

}

static void load()
{

    view.onload();

    changestate(0);

}

static void render()
{

    view.status.text.content = status[view.state];

    if (view.state == 0)
        menu_render(&view.menu);

    text_renderbox(&view.status, TEXT_COLOR_NORMAL);

}

static void confirm()
{

    void *uninstallthread = backend_createthread(uninstall, NULL);

    if (!uninstallthread)
        changestate(3);

}

static void keydown(unsigned int key)
{

    switch (key)
    {

    case KEY_A:
        if (!menu_isactive(&view.menu, view.menu.currentitem))
            break;

        switch (view.menu.currentitem)
        {

        case 0:
            confirm();

            break;

        }

        break;

    case KEY_B:
        view_quit(&view.base);

        break;

    }

}

struct view_uninstall *view_uninstall_setup(unsigned int w, unsigned int h)
{

    view_init(&view.base, load, render, keydown);
    text_init(&view.status.text, status[0]);
    box_init(&view.status.box, 0, 0, w, (4 * RENDER_ROWHEIGHT) + (2 * RENDER_PADDING));
    menu_init(&view.menu, view.menuitems, 1);
    menu_inititem(&view.menuitems[0], "Yes, I am sure");
    menu_setrow(&view.menu, 0);
    box_init(&view.menu.box, 0, h - (view.menu.total * RENDER_ROWHEIGHT) - (2 * RENDER_PADDING), w, (view.menu.total * RENDER_ROWHEIGHT) + (2 * RENDER_PADDING));

    return &view;

}

