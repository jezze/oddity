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

static void renderdefault()
{

    view.status.text.content = "Please wait...";

    menu_disable(&view.menu, 0);
    text_renderbox(&view.status, TEXT_COLOR_NORMAL);
    menu_render(&view.menu);

}

static void renderworking()
{

    view.status.text.content = "Installing...";

    menu_enable(&view.menu, 0);
    text_renderbox(&view.status, TEXT_COLOR_NORMAL);
    menu_render(&view.menu);

}

static void rendercomplete()
{

    view.status.text.content = "Install complete!";

    menu_disable(&view.menu, 0);
    text_renderbox(&view.status, TEXT_COLOR_NORMAL);
    menu_render(&view.menu);

}

static void renderfail()
{

    view.status.text.content = "Install failed!";

    menu_disable(&view.menu, 0);
    text_renderbox(&view.status, TEXT_COLOR_NORMAL);
    menu_render(&view.menu);

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
    {

        file_removepackage(packagelist->items[0].name);

        return 0;

    }

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

    ztore_setmode(renderworking);
    db_loadpackagesfromapp(&packagelist, view.app);

    if (doinstall(&packagelist))
        ztore_setmode(rendercomplete);
    else
        ztore_setmode(renderfail);

    db_freepackages(&packagelist);

    return 0;

}

static void load()
{

    void *installthread;

    view.onload();

    ztore_setmode(renderdefault);

    installthread = backend_createthread(install, NULL);

    if (!installthread)
        ztore_setmode(renderfail);

}

static void keydown(unsigned int key)
{

    menu_keydown(&view.menu, key);

    switch (key)
    {

    case KEY_B:
        view_quit(&view.base);

        break;

    }

}

static void menu_onselect()
{

    switch (view.menu.currentitem)
    {

    case 0:
        break;

    }

}

struct view_install *view_install_setup(unsigned int w, unsigned int h)
{

    view_init(&view.base, load, renderdefault, keydown);
    text_init(&view.status.text, 0);
    box_init(&view.status.box, 0, 0, w, (4 * RENDER_ROWHEIGHT) + (2 * RENDER_PADDING));
    menu_init(&view.menu, view.menuitems, 1);
    menu_inititem(&view.menuitems[0], "Cancel");
    menu_disable(&view.menu, 0);
    menu_setrow(&view.menu, 0);
    box_init(&view.menu.box, 0, h - (view.menu.total * RENDER_ROWHEIGHT) - (2 * RENDER_PADDING), w, (view.menu.total * RENDER_ROWHEIGHT) + (2 * RENDER_PADDING));

    view.menu.onselect = menu_onselect;

    return &view;

}

