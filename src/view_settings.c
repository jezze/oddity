#include <stdlib.h>
#include "define.h"
#include "backend.h"
#include "box.h"
#include "text.h"
#include "list.h"
#include "menu.h"
#include "view.h"
#include "ztore.h"
#include "area.h"

static struct view view;
static struct area areas[6];

static void place(unsigned int w, unsigned int h)
{

    area_place(&areas[0], w, h);
    area_place(&areas[1], w, h);
    area_place(&areas[2], w, h);
    area_place(&areas[3], w, h);
    area_place(&areas[4], w, h);
    area_place(&areas[5], w, h);

}

static void render(unsigned int ticks)
{

    area_render(&areas[0]);
    area_render(&areas[1]);
    area_render(&areas[2]);
    area_render(&areas[3]);
    area_render(&areas[4]);
    area_render(&areas[5]);
    text_render2(&areas[0].size, TEXT_COLOR_TITLE, TEXT_ALIGN_LEFT, "Hello world!");
    text_render2(&areas[1].size, TEXT_COLOR_NORMAL, TEXT_ALIGN_LEFT, "Hello world!\nHello world!\nHello world!");
    text_render2(&areas[2].size, TEXT_COLOR_NORMAL, TEXT_ALIGN_LEFT, "Hello world!");
    text_render2(&areas[3].size, TEXT_COLOR_SELECT, TEXT_ALIGN_LEFT, "Hello world!\nHello world!\nHello world!");
    text_render2(&areas[4].size, TEXT_COLOR_DISABLE, TEXT_ALIGN_LEFT, "Hello world!");

}

static void button(unsigned int key)
{

    switch (key)
    {

    case KEY_B:
        view_quit("settings");

        break;

    case KEY_UP:
        break;

    case KEY_DOWN:
        break;

    }

}

static void load(void)
{

    ztore_setview(place, render, button);

}

static void config(char *key, void *value)
{

}

void view_settings_setup(void)
{

    view_init(&view, load, config);
    view_register("settings", &view);
    area_init(&areas[0], 0, 0, 6, 1);
    area_init(&areas[1], 0, 1, 6, 2);
    area_init(&areas[2], 0, 3, 6, 1);
    area_init(&areas[3], 0, 4, 3, 2);
    area_init(&areas[4], 3, 4, 3, 1);
    area_init(&areas[5], 3, 5, 3, 1);

}

