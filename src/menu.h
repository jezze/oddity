#define MENUITEM_FLAG_NORMAL            0
#define MENUITEM_FLAG_DISABLED          1
#define MENUITEM_FLAG_SELECTED          2

struct menuitem
{

    char *label;
    char *info;
    unsigned int flag;

};

struct menu
{

    struct menuitem *items;
    unsigned int total;
    unsigned int currentitem;
    void (*onselect)(void);

};

void menu_setrow(struct menu *menu, unsigned int index);
void menu_enable(struct menu *menu, unsigned int index);
void menu_disable(struct menu *menu, unsigned int index);
void menu_button(struct menu *menu, unsigned int key);
void menu_renderitem(struct menuitem *menuitem, struct box *box);
void menu_render(struct menu *menu, struct box *box);
void menu_inititem(struct menuitem *menuitem, char *label, char *info);
void menu_init(struct menu *menu, struct menuitem *items, unsigned int total);
