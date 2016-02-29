#include <stdlib.h>
#include "view.h"
#include "box.h"
#include "text.h"
#include "menu.h"
#include "backend.h"

void menu_setrow(struct menu *menu, unsigned int index)
{

    menu->items[menu->currentitem].type &= ~MENUITEM_FLAG_SELECTED;
    menu->currentitem = index;
    menu->items[menu->currentitem].type |= MENUITEM_FLAG_SELECTED;

}

void menu_nextrow(struct menu *menu)
{

    if (menu->total)
        menu_setrow(menu, (menu->total + menu->currentitem + 1) % menu->total);

}

void menu_prevrow(struct menu *menu)
{

    if (menu->total)
        menu_setrow(menu, (menu->total + menu->currentitem - 1) % menu->total);

}

void menu_nextpage(struct menu *menu)
{

    unsigned int pagerows = (menu->box.h - 2 * RENDER_PADDING) / RENDER_ROWHEIGHT;
    unsigned int pagetotal = (menu->total / pagerows) + 1;
    unsigned int pageoffset = (pagetotal + (menu->currentitem / pagerows) + 1) % pagetotal;
    unsigned int rowoffset = menu->currentitem % pagerows;
    unsigned int rowstart = pageoffset * pagerows;
    unsigned int rowtotal = (menu->total - rowstart);

    menu_setrow(menu, rowstart + ((rowtotal <= rowoffset) ? rowtotal - 1 : rowoffset));

}

void menu_prevpage(struct menu *menu)
{

    unsigned int pagerows = (menu->box.h - 2 * RENDER_PADDING) / RENDER_ROWHEIGHT;
    unsigned int pagetotal = (menu->total / pagerows) + 1;
    unsigned int pageoffset = (pagetotal + (menu->currentitem / pagerows) - 1) % pagetotal;
    unsigned int rowoffset = menu->currentitem % pagerows;
    unsigned int rowstart = pageoffset * pagerows;
    unsigned int rowtotal = (menu->total - rowstart);

    menu_setrow(menu, rowstart + ((rowtotal <= rowoffset) ? rowtotal - 1 : rowoffset));

}

void menu_renderitem(struct menuitem *menuitem, int x, int y, int w, int h)
{

    if (menuitem->type & MENUITEM_FLAG_SELECTED)
        backend_rect(x, y, w, h);

    if (menuitem->type & MENUITEM_FLAG_BLOCKED)
        text_render(&menuitem->text, x, y, w, h, TEXT_COLOR_DISABLE);
    else
        text_render(&menuitem->text, x, y, w, h, TEXT_COLOR_SELECT);

}

void menu_render(struct menu *menu)
{

    unsigned int pagerows = (menu->box.h - 2 * RENDER_PADDING) / RENDER_ROWHEIGHT;
    unsigned int page = (menu->currentitem / pagerows);
    unsigned int rowstart = page * pagerows;
    unsigned int rowend = (rowstart + pagerows) > menu->total ? menu->total : rowstart + pagerows;
    unsigned int row;

    for (row = rowstart; row < rowend; row++)
        menu_renderitem(&menu->items[row], menu->box.x + RENDER_PADDING, menu->box.y + RENDER_PADDING + (row - rowstart) * RENDER_ROWHEIGHT, menu->box.w - (2 * RENDER_PADDING), RENDER_ROWHEIGHT);

}

