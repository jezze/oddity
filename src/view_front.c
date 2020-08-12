#include <stdlib.h>
#include "define.h"
#include "box.h"
#include "text.h"
#include "list.h"
#include "menu.h"
#include "view.h"
#include "ztore.h"

static struct view view;
static struct box greetingbox;
static struct menu menu;
static struct box menubox;
static struct menuitem menuitems[4];

static void place(unsigned int w, unsigned int h)
{

    box_setpartsize(&greetingbox, w / 10, h / 10, 0, 0, 10, 5);
    box_setpartsize(&menubox, w / 10, h / 10, 0, 5, 10, 5);

}

static void render(unsigned int ticks)
{

    text_render(&greetingbox, TEXT_COLOR_NORMAL, TEXT_ALIGN_LEFT, "Welcome to Ztore!");
    menu_render(&menu, &menubox);

}

static void button(unsigned int key)
{

    menu_button(&menu, key);

}

static void load(void)
{

    ztore_setview(place, render, button);

}

static void config(char *key, void *value)
{

}

static void menu_onselect(unsigned int index)
{

    switch (index)
    {

    case 0:
        view_load("repolist", "front");

        break;

    case 1:
        view_load("sync", "front");

        break;

    case 2:
        view_load("settings", "front");

        break;

    case 3:
        ztore_quit();

        break;

    }

}

void view_front_setup(void)
{

    view_init(&view, load, config);
    box_init(&greetingbox);
    box_init(&menubox);
    menu_init(&menu, menuitems, 4, menu_onselect);
    menu_inititem(&menuitems[0], "Browse", 0);
    menu_inititem(&menuitems[1], "Sync", 0);
    menu_inititem(&menuitems[2], "Settings", 0);
    menu_inititem(&menuitems[3], "Exit", 0);
    menu_setrow(&menu, 0);
    view_register("front", &view);

}

