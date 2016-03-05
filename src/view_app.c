#include <stdlib.h>
#include <string.h>
#include "define.h"
#include "box.h"
#include "text.h"
#include "menu.h"
#include "file.h"
#include "db.h"
#include "view.h"
#include "view_app.h"
#include "view_install.h"
#include "ztore.h"

static struct view_app view;

static void load()
{

    view.onload(&view.app);

    view.title.text.content = view.app.name;
    view.shortdescription.text.content = view.app.shortdescription;

    menu_setrow(&view.menu, 0);

    if (view.app.state == 3)
    {

        menu_enable(&view.menu, 0);
        menu_disable(&view.menu, 1);
        menu_enable(&view.menu, 2);

    }

    else
    {

        menu_disable(&view.menu, 0);
        menu_enable(&view.menu, 1);
        menu_disable(&view.menu, 2);

    }

}

static void render()
{

    text_renderbox(&view.title, TEXT_COLOR_TITLE);
    text_renderbox(&view.shortdescription, TEXT_COLOR_NORMAL);
    menu_render(&view.menu);

}

static void runpackage()
{

    struct db_packagelist packagelist;
    unsigned int i;

    db_loadpackagesfromapp(&packagelist, &view.app);

    for (i = 0; i < packagelist.count; i++)
    {

        if (packagelist.items[i].state != 3)
            continue;

        ztore_exec(packagelist.items[i].name);

        break;

    }

    db_freepackages(&packagelist);

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
            runpackage();

            break;

        case 1:
            ztore_load(&view.installview->base);

            break;

        }

        break;

    case KEY_B:
        view_quit(&view.base);

        break;

    }

}

static void installview_onquit()
{

    ztore_load(&view.base);

}

static void installview_onload(struct db_app *app, struct db_packagelist *packagelist)
{

    db_freeapp(app);
    db_loadapp(app, view.app.id);
    db_freepackages(packagelist);
    db_loadpackagesfromapp(packagelist, app);

}

struct view_app *view_app_setup(unsigned int w, unsigned int h)
{

    view_init(&view.base, load, render, keydown);
    menu_init(&view.menu, view.menuitems, 3);
    menu_inititem(&view.menuitems[0], "Run");
    menu_inititem(&view.menuitems[1], "Install");
    menu_inititem(&view.menuitems[2], "Uninstall");
    menu_setrow(&view.menu, 0);
    box_init(&view.title.box, 0, 0, w, (1 * RENDER_ROWHEIGHT) + (2 * RENDER_PADDING));
    box_init(&view.shortdescription.box, 0, (1 * RENDER_ROWHEIGHT), w, (4 * RENDER_ROWHEIGHT) + (2 * RENDER_PADDING));
    box_init(&view.menu.box, 0, h - (view.menu.total * RENDER_ROWHEIGHT) - (2 * RENDER_PADDING), w, (view.menu.total * RENDER_ROWHEIGHT) + (2 * RENDER_PADDING));

    view.installview = view_install_setup(w, h);
    view.installview->base.onquit = installview_onquit;
    view.installview->onload = installview_onload;

    return &view;

}

