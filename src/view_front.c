#include <stdlib.h>
#include "define.h"
#include "box.h"
#include "list.h"
#include "widget.h"
#include "view.h"
#include "main.h"

static struct view quit;
static struct view view;

static void button(unsigned int key)
{

    view_moveselection(&view, key);
    view_select(&view, key, "area_library", view.name, "repolist");
    view_select(&view, key, "area_settings", view.name, "settings");
    view_select(&view, key, "area_exit", view.name, "quit");

}

static void load(void)
{

    view_reset(&view);

}

void view_front_setup(void)
{

    widget_area_init(view_createwidget(&view), "area_library", WIDGET_IN_DEFAULT, 0, 4, 8, 1);
    widget_text_init(view_createwidget(&view), WIDGET_ID_DEFAULT, "area_library", TEXT_COLOR_SELECT, TEXT_ALIGN_CENTER, "Library");
    widget_area_init(view_createwidget(&view), "area_settings", WIDGET_IN_DEFAULT, 0, 5, 8, 1);
    widget_text_init(view_createwidget(&view), WIDGET_ID_DEFAULT, "area_settings", TEXT_COLOR_SELECT, TEXT_ALIGN_CENTER, "Settings");
    widget_area_init(view_createwidget(&view), "area_exit", WIDGET_IN_DEFAULT, 0, 6, 8, 1);
    widget_text_init(view_createwidget(&view), WIDGET_ID_DEFAULT, "area_exit", TEXT_COLOR_SELECT, TEXT_ALIGN_CENTER, "Exit");
    view_addselection(&view, "area_library");
    view_addselection(&view, "area_settings");
    view_addselection(&view, "area_exit");
    view_init(&quit, "quit", main_quit, 0, 0, 0);
    main_registerview(&quit);
    view_init(&view, "front", load, 0, 0, button);
    main_registerview(&view);

}

