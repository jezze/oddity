#include <stdlib.h>
#include "define.h"
#include "box.h"
#include "list.h"
#include "widget.h"
#include "view.h"
#include "main.h"

static struct view view;

static void button(unsigned int key)
{

    view_moveselection(&view, key);
    view_select(&view, key, "area_library", view.name, "repolist");
    view_select(&view, key, "area_settings", view.name, "settings");
    view_select(&view, key, "area_exit", view.name, "exit");

}

static void load(void)
{

    view_setattr(&view, "area_library", "selectable", "true");
    view_setattr(&view, "area_settings", "selectable", "true");
    view_setattr(&view, "area_exit", "selectable", "true");
    view_reset(&view);

}

void view_front_setup(void)
{

    widget_area_init(view_createwidget(&view), "area_library", WIDGET_IN_DEFAULT, 0, 5, 12, 1);
    widget_text_init(view_createwidget(&view), WIDGET_ID_DEFAULT, "area_library", TEXT_TYPE_SELECT, TEXT_ALIGN_CENTER, "Library");
    widget_area_init(view_createwidget(&view), "area_settings", WIDGET_IN_DEFAULT, 0, 6, 12, 1);
    widget_text_init(view_createwidget(&view), WIDGET_ID_DEFAULT, "area_settings", TEXT_TYPE_SELECT, TEXT_ALIGN_CENTER, "Settings");
    widget_area_init(view_createwidget(&view), "area_exit", WIDGET_IN_DEFAULT, 0, 7, 12, 1);
    widget_text_init(view_createwidget(&view), WIDGET_ID_DEFAULT, "area_exit", TEXT_TYPE_SELECT, TEXT_ALIGN_CENTER, "Exit");
    view_init(&view, "front", load, 0, 0, button);
    main_registerview(&view);

}

