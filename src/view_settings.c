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
    widget_text_placein(&textvideo, &areas[0].size);
    widget_text_placein(&textresolution, &areas[0].size);
    widget_text_placein(&textaudio, &areas[1].size);
    widget_text_placein(&textvolume, &areas[1].size);
    widget_text_placein(&textnetwork, &areas[2].size);
    widget_text_placein(&textaddress, &areas[2].size);
    widget_text_placein(&textsystem, &areas[3].size);

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
    widget_text_init(&textvideo, TEXT_COLOR_SELECT, TEXT_ALIGN_LEFT, "Video");
    widget_text_init(&textresolution, TEXT_COLOR_NORMAL, TEXT_ALIGN_RIGHT, "320x240");
    widget_text_init(&textaudio, TEXT_COLOR_SELECT, TEXT_ALIGN_LEFT, "Audio");
    widget_text_init(&textvolume, TEXT_COLOR_NORMAL, TEXT_ALIGN_RIGHT, "60%");
    widget_text_init(&textnetwork, TEXT_COLOR_SELECT, TEXT_ALIGN_LEFT, "Network");
    widget_text_init(&textaddress, TEXT_COLOR_NORMAL, TEXT_ALIGN_RIGHT, "10.1.1.2/30");
    widget_text_init(&textsystem, TEXT_COLOR_SELECT, TEXT_ALIGN_LEFT, "System");
    list_add(&selection.list, &areas[0].item);
    list_add(&selection.list, &areas[1].item);
    list_add(&selection.list, &areas[2].item);
    list_add(&selection.list, &areas[3].item);

}

