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
static struct widget systemarea;
static struct widget systemareaicon;
static struct widget systemareatext;
static struct widget systemtext;

static void button(unsigned int key)
{

    view_moveselection(&view, key);
    view_select(&view, key, "area_audio", view.name, "settings_audio");
    view_select(&view, key, "area_system", view.name, "settings_system");
    view_unselect(&view, key, view.name);

}

static void load(void)
{

    view_reset(&view);

}

void view_settings_setup(void)
{

    widget_area_init(&audioarea, "area_audio", WIDGET_IN_DEFAULT, 0, 0, 4, 3);
    widget_area_init(&audioareaicon, "area_audio_icon", WIDGET_IN_DEFAULT, 0, 0, 4, 2);
    widget_area_init(&audioareatext, "area_audio_text", WIDGET_IN_DEFAULT, 0, 2, 4, 1);
    widget_text_init(&audiotext, WIDGET_ID_DEFAULT, "area_audio_text", TEXT_COLOR_SELECT, TEXT_ALIGN_CENTER, "Audio");
    widget_area_init(&systemarea, "area_system", WIDGET_IN_DEFAULT, 4, 0, 4, 3);
    widget_area_init(&systemareaicon, "area_system_icon", WIDGET_IN_DEFAULT, 4, 0, 4, 2);
    widget_area_init(&systemareatext, "area_system_text", WIDGET_IN_DEFAULT, 4, 2, 4, 1);
    widget_text_init(&systemtext, WIDGET_ID_DEFAULT, "area_system_text", TEXT_COLOR_SELECT, TEXT_ALIGN_CENTER, "System");
    view_addselection(&view, &audioarea);
    view_addselection(&view, &systemarea);
    view_init(&view, "settings", load, 0, 0, button);
    view_register(&view, &audioarea);
    view_register(&view, &audioareaicon);
    view_register(&view, &audioareatext);
    view_register(&view, &audiotext);
    view_register(&view, &systemarea);
    view_register(&view, &systemareaicon);
    view_register(&view, &systemareatext);
    view_register(&view, &systemtext);
    main_registerview(&view);

}

