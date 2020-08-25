#include <stdlib.h>
#include "define.h"
#include "box.h"
#include "list.h"
#include "widget.h"
#include "selection.h"
#include "view.h"
#include "main.h"

static struct view view;
static struct widget audioarea;
static struct widget audioareaicon;
static struct widget audioareatext;
static struct widget audiotext;

static void button(unsigned int key)
{

    selection_move(&view.selection, key);
    selection_select(&view.selection, key, "audioarea", view.name, "settings_audio");
    selection_unselect(&view.selection, key, view.name);

}

static void load(void)
{

    selection_reset(&view.selection);

}

void view_settings_setup(void)
{

    widget_area_init(&audioarea, "audioarea", "main", 0, 0, 4, 3);
    widget_area_init(&audioareaicon, "", "main", 0, 0, 4, 2);
    widget_area_init(&audioareatext, "audioareatext", "main", 0, 2, 4, 1);
    widget_text_init(&audiotext, "", "audioareatext", TEXT_COLOR_SELECT, TEXT_ALIGN_CENTER, "Audio");
    selection_add(&view.selection, &audioarea);
    view_init(&view, "settings", load, 0, 0, button);
    view_register(&view, &audioarea);
    view_register(&view, &audioareaicon);
    view_register(&view, &audioareatext);
    view_register(&view, &audiotext);
    main_register(&view);

}

