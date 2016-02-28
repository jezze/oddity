#define MENUITEM_FLAG_NORMAL            0
#define MENUITEM_FLAG_BLOCKED           1
#define MENUITEM_FLAG_SELECTED          2

struct menuitem
{

    struct text text;
    unsigned int type;

};

struct menu
{

    struct menuitem *items;
    unsigned int total;
    unsigned int currentitem;
    struct box box;

};

void menu_setrow(struct menu *menu, unsigned int index);
void menu_nextrow(struct menu *menu);
void menu_prevrow(struct menu *menu);
void menu_nextpage(struct menu *menu);
void menu_prevpage(struct menu *menu);
void menu_renderitem(struct menuitem *menuitem, int x, int y, int w, int h);
void menu_render(struct menu *menu);
