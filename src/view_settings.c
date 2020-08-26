#include <stdlib.h>
#include "define.h"
#include "box.h"
#include "list.h"
#include "widget.h"
#include "view.h"
#include "main.h"

static struct view view;
static struct widget audioarea;
static struct widget audioareaicon;
static struct widget audioareatext;
static struct widget audiotext;

static void button(unsigned int key)
{

    view_moveselection(&view, key);
    view_select(&view, key, "audioarea", view.name, "settings_audio");
    view_unselect(&view, key, view.name);

}

static void load(void)
{

    view_reset(&view);

}

void view_settings_setup(void)
{

    widget_area_init(&audioarea, "audioarea", WIDGET_IN_DEFAULT, 0, 0, 4, 3);
    widget_area_init(&audioareaicon, "audioareaicon", WIDGET_IN_DEFAULT, 0, 0, 4, 2);
    widget_area_init(&audioareatext, "audioareatext", WIDGET_IN_DEFAULT, 0, 2, 4, 1);
    widget_text_init(&audiotext, WIDGET_ID_DEFAULT, "audioareatext", TEXT_COLOR_SELECT, TEXT_ALIGN_CENTER, "Audio");
    view_addselection(&view, &audioarea);
    view_init(&view, "settings", load, 0, 0, button);
    view_register(&view, &audioarea);
    view_register(&view, &audioareaicon);
    view_register(&view, &audioareatext);
    view_register(&view, &audiotext);
    main_registerview(&view);

}

