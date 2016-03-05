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

static unsigned int installcheck()
{

    struct db_packagelist packagelist;
    unsigned int changed = 0;
    unsigned int i;

    db_loadpackagesfromapp(&packagelist, view.app);

    for (i = 0; i < packagelist.count; i++)
    {

        char path[64];

        file_getpackagepath(path, 64, packagelist.items[i].name);

        if (file_exist(path) && file_matchsha1(path, packagelist.items[i].sha1))
        {

            view.app->state = 3;

            db_saveappstate(view.app);

            packagelist.items[i].state = 3;

            db_savepackagestate(&packagelist.items[i]);

            changed = 1;

        }

    }

    db_freepackages(&packagelist);

    return changed;

}

static int install(void *arg)
{

    menu_enable(&view.menu, 0);
    changestate(1);

    if (installcheck())
    {

        menu_disable(&view.menu, 0);
        changestate(2);

    }

    else
    {

        menu_disable(&view.menu, 0);
        changestate(3);

    }

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

