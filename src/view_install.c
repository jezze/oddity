#include <stdlib.h>
#include "define.h"
#include "box.h"
#include "text.h"
#include "menu.h"
#include "file.h"
#include "db.h"
#include "view.h"
#include "view_install.h"
#include "backend.h"
#include "ztore.h"

static struct view_install view;
static char *status[4] = {
    "Please wait...",
    "Installing...",
    "Install complete!",
    "Install failed!"
};

static void changestate(unsigned int state)
{

    view.state = state;

    ztore_redraw();

}

static void updatestates(struct db_package *package)
{

    view.app->state = 3;

    db_saveappstate(view.app);

    package->state = 3;

    db_savepackagestate(package);

}

static unsigned int checkpackageexist(struct db_package *package)
{

    char path[64];

    file_getpackagepath(path, 64, package->name);

    return file_exist(path) && file_matchsha1(path, package->sha1);

}

static unsigned int checkexist(struct db_packagelist *packagelist)
{

    unsigned int i;

    for (i = 0; i < packagelist->count; i++)
    {

        if (checkpackageexist(&packagelist->items[i]))
        {

            updatestates(&packagelist->items[i]);

            return 1;

        }

    }

    return 0;

}

static unsigned int doinstall(struct db_packagelist *packagelist)
{

    if (!packagelist->count)
        return 0;

    if (checkexist(packagelist))
        return 1;

    if (!file_downloadpackage(packagelist->items[0].name))
        return 0;

    if (checkpackageexist(&packagelist->items[0]))
    {

        updatestates(&packagelist->items[0]);

        return 1;

    }

    return 0;

}

static int install(void *arg)
{

    struct db_packagelist packagelist;

    changestate(1);
    db_loadpackagesfromapp(&packagelist, view.app);

    if (doinstall(&packagelist))
        changestate(2);
    else
        changestate(3);

    db_freepackages(&packagelist);

    return 0;

}

static void load()
{

    void *installthread;

    changestate(0);

    view.onload();

    installthread = backend_createthread(install, NULL);

    if (!installthread)
        changestate(3);

}

static void render()
{

    view.status.text.content = status[view.state];

    if (view.state == 1)
        menu_enable(&view.menu, 0);
    else
        menu_disable(&view.menu, 0);

    text_renderbox(&view.status, TEXT_COLOR_NORMAL);
    menu_render(&view.menu);

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
            break;

        }

        break;

    case KEY_B:
        view_quit(&view.base);

        break;

    }

}

struct view_install *view_install_setup(unsigned int w, unsigned int h)
{

    view_init(&view.base, load, render, keydown);
    text_init(&view.status.text, status[0]);
    box_init(&view.status.box, 0, 0, w, (4 * RENDER_ROWHEIGHT) + (2 * RENDER_PADDING));
    menu_init(&view.menu, view.menuitems, 1);
    menu_inititem(&view.menuitems[0], "Cancel");
    menu_disable(&view.menu, 0);
    menu_setrow(&view.menu, 0);
    box_init(&view.menu.box, 0, h - (view.menu.total * RENDER_ROWHEIGHT) - (2 * RENDER_PADDING), w, (view.menu.total * RENDER_ROWHEIGHT) + (2 * RENDER_PADDING));

    return &view;

}

