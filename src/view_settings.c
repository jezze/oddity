#include <stdlib.h>
#include "define.h"
#include "box.h"
#include "list.h"
#include "view.h"
#include "main.h"
#include "widget.h"
#include "selection.h"

static struct view view;
static struct widget_area audioarea;
static struct widget_text audiotext;
static struct selection selection;

static void place(unsigned int w, unsigned int h)
{

    widget_area_place(&audioarea, 0, 0, w, h);
    widget_text_placein(&audiotext, &audioarea.size);

}

static void render(unsigned int ticks)
{

    selection_render(&selection, ticks);
    widget_text_render(&audiotext, ticks);

}

static void button(unsigned int key)
{

    selection_setclosest(&selection, key);

    if (selection_isactive(&selection, &audioarea.item))
        selection_select(&selection, key, "settings", "settings_audio");

    selection_unselect(&selection, key, "settings");

}

static void load(void)
{

    main_setview(place, render, button);
    selection_reset(&selection);

}

void view_settings_setup(void)
{

    view_init(&view, "settings", load, 0);
    view_register(&view);
    widget_area_init(&audioarea, 0, 7, 8, 1);
    widget_text_init(&audiotext, TEXT_COLOR_SELECT, TEXT_ALIGN_LEFT, "Audio");
    selection_add(&selection, &audioarea.item);

}

