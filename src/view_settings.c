#include <stdlib.h>
#include "define.h"
#include "box.h"
#include "list.h"
#include "view.h"
#include "main.h"
#include "widget.h"
#include "selection.h"

static struct view view;
static struct widget_area areas[2];
static struct widget_text texts[1];
static struct widget_area volume_area_text;
static struct widget_area volume_area_slider;
static struct widget_text volume_text;
static struct widget_slider volume_slider;
static struct selection selection;

static void place(unsigned int w, unsigned int h)
{

    widget_area_place(&areas[0], 0, 0, w, h);
    widget_area_place(&areas[1], 0, 0, w, h);
    widget_text_placein(&texts[0], &areas[0].size);

    /* Volume */
    widget_area_place(&volume_area_text, 0, 0, w, h);
    widget_area_place(&volume_area_slider, 0, 0, w, h);
    widget_text_placein(&volume_text, &volume_area_text.size);
    widget_slider_placein(&volume_slider, &volume_area_slider.size);

}

static void render(unsigned int ticks)
{

    widget_area_render(selection.active->data);
    widget_text_render(&texts[0]);
    widget_text_render(&volume_text);
    widget_slider_render(&volume_slider);

}

static void button(unsigned int key)
{

    selection_setclosest(&selection, key);
    selection_unselect(&selection, key, "settings");

}

static void load(void)
{

    main_setview(place, render, button);

    selection.active = selection.list.head;

}

void view_settings_setup(void)
{

    view_init(&view, "settings", load, 0);
    view_register(&view);
    widget_area_init(&areas[0], 0, 0, 8, 1);
    widget_area_init(&areas[1], 0, 1, 8, 1);
    selection_add(&selection, &areas[1].item);
    widget_text_init(&texts[0], TEXT_COLOR_TITLE, TEXT_ALIGN_LEFT, "Audio");

    /* Volume */
    widget_area_init(&volume_area_text, 0, 1, 5, 1);
    widget_area_init(&volume_area_slider, 5, 1, 3, 1);
    widget_text_init(&volume_text, TEXT_COLOR_NORMAL, TEXT_ALIGN_LEFT, "Volume");
    widget_slider_init(&volume_slider, 0, 31, 26);

}

