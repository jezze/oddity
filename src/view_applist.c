#include <stdlib.h>
#include "define.h"
#include "box.h"
#include "text.h"
#include "menu.h"
#include "db.h"
#include "view.h"
#include "view_app.h"
#include "view_applist.h"
#include "ztore.h"

static struct view view;
static struct view *appview;
static struct menu menu;
static struct box menubox;
static struct box emptytextbox;
static struct db_applist *applist;

static void render(void)
{

    if (applist->count)
        menu_render(&menu, &menubox);
    else
        text_render(&emptytextbox, TEXT_COLOR_NORMAL, TEXT_ALIGN_LEFT, "No items found.");

}

static void button(unsigned int key)
{

    menu_button(&menu, key);

    switch (key)
    {

    case KEY_B:
        view_quit(&view);

        break;

    }

}

static void load(void)
{

    unsigned int i;
    char *stateinfo[] = {0, "New", "Updated", "Installed"};

    free(menu.items);

    menu.items = malloc(sizeof (struct menuitem) * applist->count);
    menu.total = applist->count;

    for (i = 0; i < menu.total; i++)
        menu_inititem(&menu.items[i], applist->items[i].name, stateinfo[applist->items[i].state]);

    menu_setrow(&menu, 0);
    ztore_setview(render, button);

}

static void menu_onselect(void)
{

    if (applist->count)
    {

        view_app_set(&applist->items[menu.currentitem]);
        view_load(appview, &view);

    }

}

void view_applist_setlist(struct db_applist *list)
{

    applist = list;

}

struct view *view_applist_setup(unsigned int w, unsigned int h)
{

    view_init(&view, load);
    box_init(&emptytextbox);
    box_init(&menubox);
    box_setpartsize(&emptytextbox, w, h, 0, 0, 1, 1);
    box_setpartsize(&menubox, w, h, 0, 0, 1, 1);
    menu_init(&menu, 0, 0);

    menu.onselect = menu_onselect;
    appview = view_app_setup(w, h);

    return &view;

}

