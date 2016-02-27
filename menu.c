#include <stdlib.h>
#include "ztore.h"
#include "text.h"
#include "menu.h"

void menu_setrow(struct menu *menu, unsigned int index)
{

    menu->items[menu->currentitem].type &= ~MENUITEM_FLAG_SELECTED;
    menu->currentitem = index;
    menu->items[menu->currentitem].type |= MENUITEM_FLAG_SELECTED;

}

void menu_nextrow(struct menu *menu)
{

    menu_setrow(menu, (menu->total + menu->currentitem + 1) % menu->total);

}

void menu_prevrow(struct menu *menu)
{

    menu_setrow(menu, (menu->total + menu->currentitem - 1) % menu->total);

}

void menu_nextpage(struct menu *menu)
{

    unsigned int pagerows = (menu->box.h - 2 * MENU_PADDING) / MENU_ROWHEIGHT;
    unsigned int pagetotal = (menu->total / pagerows) + 1;
    unsigned int pageoffset = (pagetotal + (menu->currentitem / pagerows) + 1) % pagetotal;
    unsigned int rowoffset = menu->currentitem % pagerows;
    unsigned int rowstart = pageoffset * pagerows;
    unsigned int rowtotal = (menu->total - rowstart);

    menu_setrow(menu, rowstart + ((rowtotal <= rowoffset) ? rowtotal - 1 : rowoffset));

}

void menu_prevpage(struct menu *menu)
{

    unsigned int pagerows = (menu->box.h - 2 * MENU_PADDING) / MENU_ROWHEIGHT;
    unsigned int pagetotal = (menu->total / pagerows) + 1;
    unsigned int pageoffset = (pagetotal + (menu->currentitem / pagerows) - 1) % pagetotal;
    unsigned int rowoffset = menu->currentitem % pagerows;
    unsigned int rowstart = pageoffset * pagerows;
    unsigned int rowtotal = (menu->total - rowstart);

    menu_setrow(menu, rowstart + ((rowtotal <= rowoffset) ? rowtotal - 1 : rowoffset));

}

