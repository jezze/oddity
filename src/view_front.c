#include <stdlib.h>
#include "view.h"
#include "box.h"
#include "text.h"
#include "menu.h"
#include "backend.h"
#include "ztore.h"

static struct menuitem menuitems[3] = {
    {{"Browse"}, MENUITEM_FLAG_NORMAL},
    {{"Settings"}, MENUITEM_FLAG_BLOCKED},
    {{"Exit"}, MENUITEM_FLAG_NORMAL}
};

static struct view view;
static struct view *categoryview;
static struct textbox text;
static struct menu menu;

static void show()
{

}

static void hide()
{

}

static void render()
{

    text_renderbox(&text, TEXT_COLOR_NORMAL);
    menu_render(&menu);

}

static void keydown(unsigned int key)
{

    switch (key)
    {

    case KEY_UP:
        menu_prevrow(&menu);

        break;

    case KEY_DOWN:
        menu_nextrow(&menu);

        break;

    case KEY_A:
        switch (menu.currentitem)
        {

        case 0:
            ztore_flipview(categoryview);

            break;

        case 2:
            ztore_quit();

            break;

        }

        break;

    case KEY_B:
        ztore_flipview(view.parent);

        break;

    }

}

struct view *view_frontsetup(unsigned int w, unsigned int h, struct view *category)
{

    view_init(&view, show, hide, render, keydown);

    categoryview = category;
    text.text.content = "Welcome to Ztore!";
    menu.items = menuitems;
    menu.total = 3;

    menu_setrow(&menu, 0);
    box_init(&text.box, 0, 0, w, (4 * RENDER_ROWHEIGHT) + (2 * RENDER_PADDING));
    box_init(&menu.box, 0, h - (menu.total * RENDER_ROWHEIGHT) - (2 * RENDER_PADDING), w, (menu.total * RENDER_ROWHEIGHT) + (2 * RENDER_PADDING));

    return &view;

}

