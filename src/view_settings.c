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
static struct widget_text textvideo;
static struct widget_text textresolution;
static struct widget_text textaudio;
static struct widget_text textvolume;
static struct widget_text textnetwork;
static struct widget_text textaddress;
static struct widget_text textsystem;
static struct selection selection;

static void place(unsigned int w, unsigned int h)
{

    widget_area_place(&areas[0], 0, 0, w, h);
    widget_area_place(&areas[1], 0, 0, w, h);
    widget_area_place(&areas[2], 0, 0, w, h);
    widget_area_place(&areas[3], 0, 0, w, h);
    widget_text_place(&textvideo, areas[0].size.x, areas[0].size.y, areas[0].size.w, areas[0].size.h);
    widget_text_place(&textresolution, areas[0].size.x, areas[0].size.y, areas[0].size.w, areas[0].size.h);
    widget_text_place(&textaudio, areas[1].size.x, areas[1].size.y, areas[1].size.w, areas[1].size.h);
    widget_text_place(&textvolume, areas[1].size.x, areas[1].size.y, areas[1].size.w, areas[1].size.h);
    widget_text_place(&textnetwork, areas[2].size.x, areas[2].size.y, areas[2].size.w, areas[2].size.h);
    widget_text_place(&textaddress, areas[2].size.x, areas[2].size.y, areas[2].size.w, areas[2].size.h);
    widget_text_place(&textsystem, areas[3].size.x, areas[3].size.y, areas[3].size.w, areas[3].size.h);

}

static void render(unsigned int ticks)
{

    widget_area_render(selection.active->data);
    widget_text_render(&textvideo);
    widget_text_render(&textresolution);
    widget_text_render(&textaudio);
    widget_text_render(&textvolume);
    widget_text_render(&textnetwork);
    widget_text_render(&textaddress);
    widget_text_render(&textsystem);

}

static void button(unsigned int key)
{

    selection_setclosest(&selection, key);

    switch (key)
    {

    case KEY_B:
        view_quit("settings");

        break;

    }

}

static void load(void)
{

    ztore_setview(place, render, button);
    list_add(&selection.list, &areas[0].item);
    list_add(&selection.list, &areas[1].item);
    list_add(&selection.list, &areas[2].item);
    list_add(&selection.list, &areas[3].item);

    selection.active = selection.list.head;

}

static void config(char *key, void *value)
{

}

void view_settings_setup(void)
{

    view_init(&view, load, config);
    view_register("settings", &view);
    widget_area_init(&areas[0], 0, 0, 8, 1);
    widget_area_init(&areas[1], 0, 1, 8, 1);
    widget_area_init(&areas[2], 0, 2, 8, 1);
    widget_area_init(&areas[3], 0, 3, 8, 1);
    widget_text_init(&textvideo, TEXT_COLOR_SELECT, TEXT_ALIGN_LEFT, "Video");
    widget_text_init(&textresolution, TEXT_COLOR_NORMAL, TEXT_ALIGN_RIGHT, "320x240");
    widget_text_init(&textaudio, TEXT_COLOR_SELECT, TEXT_ALIGN_LEFT, "Audio");
    widget_text_init(&textvolume, TEXT_COLOR_NORMAL, TEXT_ALIGN_RIGHT, "60%");
    widget_text_init(&textnetwork, TEXT_COLOR_SELECT, TEXT_ALIGN_LEFT, "Network");
    widget_text_init(&textaddress, TEXT_COLOR_NORMAL, TEXT_ALIGN_RIGHT, "10.1.1.2/30");
    widget_text_init(&textsystem, TEXT_COLOR_SELECT, TEXT_ALIGN_LEFT, "System");

}

