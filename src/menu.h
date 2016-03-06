#define MENUITEM_FLAG_NORMAL            0
#define MENUITEM_FLAG_DISABLED          1
#define MENUITEM_FLAG_SELECTED          2

struct menuitem
{

    struct text label;
    struct text info;
    unsigned int flag;

};

struct menu
{

    struct menuitem *items;
    unsigned int total;
    unsigned int currentitem;
    struct box box;
    void (*onselect)();

};

void menu_setrow(struct menu *menu, unsigned int index);
void menu_enable(struct menu *menu, unsigned int index);
void menu_disable(struct menu *menu, unsigned int index);
void menu_keydown(struct menu *menu, unsigned int key);
void menu_renderitem(struct menuitem *menuitem, int x, int y, int w, int h);
void menu_render(struct menu *menu);
void menu_inititem(struct menuitem *menuitem, char *label, char *info);
void menu_init(struct menu *menu, struct menuitem *items, unsigned int total);
