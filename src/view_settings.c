#include <stdlib.h>
#include "define.h"
#include "box.h"
#include "list.h"
#include "view.h"
#include "main.h"
#include "widget.h"
#include "selection.h"

static struct view view;
static struct widget audioarea;
static struct widget audioareaicon;
static struct widget audioareatext;
static struct widget audiotext;
static struct selection selection;

static void place(struct box *size)
{

    widget_area_place(&audioarea, size);
    widget_area_place(&audioareaicon, size);
    widget_area_place(&audioareatext, size);
    widget_text_place(&audiotext, &audioareatext.size);

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
    widget_area_init(&audioarea, 0, 0, 4, 3);
    widget_area_init(&audioareaicon, 0, 0, 4, 2);
    widget_area_init(&audioareatext, 0, 2, 4, 1);
    widget_text_init(&audiotext, TEXT_COLOR_SELECT, TEXT_ALIGN_CENTER, "Audio");
    selection_add(&selection, &audioarea.item);

}

