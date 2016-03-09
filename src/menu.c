#include <stdlib.h>
#include "define.h"
#include "box.h"
#include "text.h"
#include "menu.h"
#include "view.h"
#include "backend.h"

static unsigned int iscurrent(struct menu *menu, unsigned int index)
{

    return menu->currentitem == index && !(menu->items[menu->currentitem].flag & MENUITEM_FLAG_DISABLED);

}

void menu_setrow(struct menu *menu, unsigned int index)
{

    menu->items[menu->currentitem].flag &= ~MENUITEM_FLAG_SELECTED;
    menu->currentitem = index;
    menu->items[menu->currentitem].flag |= MENUITEM_FLAG_SELECTED;

}

void menu_enable(struct menu *menu, unsigned int index)
{

    menu->items[index].flag &= ~MENUITEM_FLAG_DISABLED;

}

void menu_disable(struct menu *menu, unsigned int index)
{

    menu->items[index].flag |= MENUITEM_FLAG_DISABLED;

}

static void setnextrow(struct menu *menu)
{

    if (menu->total)
        menu_setrow(menu, (menu->total + menu->currentitem + 1) % menu->total);

}

static void setprevrow(struct menu *menu)
{

    if (menu->total)
        menu_setrow(menu, (menu->total + menu->currentitem - 1) % menu->total);

}

static void setnextpage(struct menu *menu)
{

    unsigned int pagerows = 9;
    unsigned int pagetotal = (menu->total / pagerows) + 1;
    unsigned int pageoffset = (pagetotal + (menu->currentitem / pagerows) + 1) % pagetotal;
    unsigned int rowoffset = menu->currentitem % pagerows;
    unsigned int rowstart = pageoffset * pagerows;
    unsigned int rowtotal = (menu->total - rowstart);

    menu_setrow(menu, rowstart + ((rowtotal <= rowoffset) ? rowtotal - 1 : rowoffset));

}

static void setprevpage(struct menu *menu)
{

    unsigned int pagerows = 9;
    unsigned int pagetotal = (menu->total / pagerows) + 1;
    unsigned int pageoffset = (pagetotal + (menu->currentitem / pagerows) - 1) % pagetotal;
    unsigned int rowoffset = menu->currentitem % pagerows;
    unsigned int rowstart = pageoffset * pagerows;
    unsigned int rowtotal = (menu->total - rowstart);

    menu_setrow(menu, rowstart + ((rowtotal <= rowoffset) ? rowtotal - 1 : rowoffset));

}

void menu_keydown(struct menu *menu, unsigned int key)
{

    switch (key)
    {

    case KEY_UP:
        setprevrow(menu);

        break;

    case KEY_DOWN:
        setnextrow(menu);

        break;

    case KEY_LEFT:
        if (menu->total > 8)
            setprevpage(menu);

        break;

    case KEY_RIGHT:
        if (menu->total > 8)
            setnextpage(menu);

        break;

    case KEY_A:
        if (iscurrent(menu, menu->currentitem) && menu->onselect)
            menu->onselect();

        break;

    }

}

void menu_renderitem(struct menuitem *menuitem, struct box *box)
{

    if (menuitem->flag & MENUITEM_FLAG_SELECTED)
        backend_rect(box->x, box->y, box->w, box->h);

    if (menuitem->flag & MENUITEM_FLAG_DISABLED)
    {

        text_render(box, TEXT_COLOR_DISABLE, TEXT_ALIGN_LEFT, menuitem->label);

        if (menuitem->info)
            text_render(box, TEXT_COLOR_DISABLE, TEXT_ALIGN_RIGHT, menuitem->info);

    }

    else
    {

        text_render(box, TEXT_COLOR_SELECT, TEXT_ALIGN_LEFT, menuitem->label);

        if (menuitem->info)
            text_render(box, TEXT_COLOR_NORMAL, TEXT_ALIGN_RIGHT, menuitem->info);

    }

}

void menu_render(struct menu *menu, struct box *box)
{

    unsigned int pagerows = 9;
    unsigned int page = (menu->currentitem / pagerows);
    unsigned int rowstart = page * pagerows;
    unsigned int rowend = (rowstart + pagerows) > menu->total ? menu->total : rowstart + pagerows;
    unsigned int row;

    for (row = rowstart; row < rowend; row++)
    {

        struct box rowbox;

        rowbox.x = box->x;
        rowbox.y = box->y + (row - rowstart) * RENDER_ROWHEIGHT;
        rowbox.w = box->w;
        rowbox.h = RENDER_ROWHEIGHT;

        menu_renderitem(&menu->items[row], &rowbox);

    }

}

void menu_inititem(struct menuitem *menuitem, char *label, char *info)
{

    menuitem->label = label;
    menuitem->info = info;
    menuitem->flag = MENUITEM_FLAG_NORMAL;

}

void menu_init(struct menu *menu, struct menuitem *items, unsigned int total)
{

    menu->items = items;
    menu->total = total;

}

