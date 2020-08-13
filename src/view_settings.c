#include <stdlib.h>
#include "define.h"
#include "box.h"
#include "text.h"
#include "list.h"
#include "view.h"
#include "ztore.h"
#include "widget.h"
#include "selection.h"

static struct view view;
static struct widget_area areas[4];
static struct widget_text texts[7];
static struct selection selection;

static void place(unsigned int w, unsigned int h)
{

    widget_area_place(&areas[0], 0, 0, w, h);
    widget_area_place(&areas[1], 0, 0, w, h);
    widget_area_place(&areas[2], 0, 0, w, h);
    widget_area_place(&areas[3], 0, 0, w, h);
    widget_text_placein(&texts[0], &areas[0].size);
    widget_text_placein(&texts[1], &areas[0].size);
    widget_text_placein(&texts[2], &areas[1].size);
    widget_text_placein(&texts[3], &areas[1].size);
    widget_text_placein(&texts[4], &areas[2].size);
    widget_text_placein(&texts[5], &areas[2].size);
    widget_text_placein(&texts[6], &areas[3].size);

}

static void render(unsigned int ticks)
{

    widget_area_render(selection.active->data);
    widget_text_render(&texts[0]);
    widget_text_render(&texts[1]);
    widget_text_render(&texts[2]);
    widget_text_render(&texts[3]);
    widget_text_render(&texts[4]);
    widget_text_render(&texts[5]);
    widget_text_render(&texts[6]);

}

static void button(unsigned int key)
{

    selection_setclosest(&selection, key);
    selection_return(&selection, key, "settings");

}

static void load(void)
{

    ztore_setview(place, render, button);

    selection.active = selection.list.head;

}

void view_settings_setup(void)
{

    view_init(&view, "settings", load, 0);
    view_register(&view);
    widget_area_init(&areas[0], 0, 0, 8, 1);
    widget_area_init(&areas[1], 0, 1, 8, 1);
    widget_area_init(&areas[2], 0, 2, 8, 1);
    widget_area_init(&areas[3], 0, 3, 8, 1);
    widget_text_init(&texts[0], TEXT_COLOR_SELECT, TEXT_ALIGN_LEFT, "Video");
    widget_text_init(&texts[1], TEXT_COLOR_NORMAL, TEXT_ALIGN_RIGHT, "320x240");
    widget_text_init(&texts[2], TEXT_COLOR_SELECT, TEXT_ALIGN_LEFT, "Audio");
    widget_text_init(&texts[3], TEXT_COLOR_NORMAL, TEXT_ALIGN_RIGHT, "60%");
    widget_text_init(&texts[4], TEXT_COLOR_SELECT, TEXT_ALIGN_LEFT, "Network");
    widget_text_init(&texts[5], TEXT_COLOR_NORMAL, TEXT_ALIGN_RIGHT, "10.1.1.2/30");
    widget_text_init(&texts[6], TEXT_COLOR_SELECT, TEXT_ALIGN_LEFT, "System");
    list_add(&selection.list, &areas[0].item);
    list_add(&selection.list, &areas[1].item);
    list_add(&selection.list, &areas[2].item);
    list_add(&selection.list, &areas[3].item);

}

