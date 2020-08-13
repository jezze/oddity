#include <stdlib.h>
#include <string.h>
#include "define.h"
#include "box.h"
#include "text.h"
#include "list.h"
#include "menu.h"
#include "db.h"
#include "view.h"
#include "ztore.h"

static struct view view;
static struct menu menu;
static struct box menubox;
static struct box emptytextbox;
static struct db_applist *applist;

static void place(unsigned int w, unsigned int h)
{

    box_setpartsize(&emptytextbox, w, h, 0, 0, 1, 1);
    box_setpartsize(&menubox, w, h, 0, 0, 1, 1);

}

static void render(unsigned int ticks)
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
        view_quit("applist");

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
    ztore_setview(place, render, button);

}

static void config(char *key, void *value)
{

    if (!strcmp(key, "list"))
        applist = value;

}

static void menu_onselect(unsigned int index)
{

    if (applist->count)
    {

        view_config("app", "app", &applist->items[index]);
        view_load("app", "applist");

    }

}

void view_applist_setup(void)
{

    view_init(&view, "applist", load, config);
    box_init(&emptytextbox);
    box_init(&menubox);
    menu_init(&menu, 0, 0, menu_onselect);
    view_register(&view);

}

