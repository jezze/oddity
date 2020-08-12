#include <stdlib.h>
#include "define.h"
#include "backend.h"
#include "box.h"
#include "text.h"
#include "list.h"
#include "menu.h"
#include "view.h"
#include "ztore.h"
#include "widget.h"

static struct view view;
static struct widget_area areas[6];
static struct widget_text texts[5];
static unsigned int active;

static void place(unsigned int w, unsigned int h)
{

    widget_area_place(&areas[0], 0, 0, w, h);
    widget_area_place(&areas[1], 0, 0, w, h);
    widget_area_place(&areas[2], 0, 0, w, h);
    widget_area_place(&areas[3], 0, 0, w, h);
    widget_text_place(&texts[0], areas[0].size.x, areas[0].size.y, areas[0].size.w, areas[0].size.h);
    widget_text_place(&texts[1], areas[1].size.x, areas[1].size.y, areas[1].size.w, areas[1].size.h);
    widget_text_place(&texts[2], areas[2].size.x, areas[2].size.y, areas[2].size.w, areas[2].size.h);
    widget_text_place(&texts[3], areas[3].size.x, areas[3].size.y, areas[3].size.w, areas[3].size.h);

}

static void render(unsigned int ticks)
{

    widget_area_render(&areas[active]);
    widget_text_render(&texts[0]);
    widget_text_render(&texts[1]);
    widget_text_render(&texts[2]);
    widget_text_render(&texts[3]);

}

static void button(unsigned int key)
{

    switch (key)
    {

    case KEY_B:
        view_quit("settings");

        break;

    case KEY_UP:
        if (active > 0)
            active--;

        break;

    case KEY_DOWN:
        if (active < 3)
            active++;

        break;

    }

}

static void load(void)
{

    ztore_setview(place, render, button);

    active = 0;

}

static void config(char *key, void *value)
{

}

void view_settings_setup(void)
{

    view_init(&view, load, config);
    view_register("settings", &view);
    widget_area_init(&areas[0], 0, 0, 2, 2);
    widget_area_init(&areas[1], 2, 0, 2, 2);
    widget_area_init(&areas[2], 4, 0, 2, 2);
    widget_area_init(&areas[3], 0, 2, 2, 2);
    widget_text_init(&texts[0], TEXT_COLOR_NORMAL, TEXT_ALIGN_LEFT, "Video");
    widget_text_init(&texts[1], TEXT_COLOR_NORMAL, TEXT_ALIGN_LEFT, "Audio");
    widget_text_init(&texts[2], TEXT_COLOR_NORMAL, TEXT_ALIGN_LEFT, "Network");
    widget_text_init(&texts[3], TEXT_COLOR_NORMAL, TEXT_ALIGN_LEFT, "System");

}

