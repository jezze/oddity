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
    view_select(&view, key, "area_audio", view.name, "settings_audio");
    view_select(&view, key, "area_system", view.name, "settings_system");
    view_unselect(&view, key, view.name);

}

static void load(void)
{

    view_setattr(&view, "area_audio", "selectable", "true");
    view_setattr(&view, "area_network", "selectable", "true");
    view_setattr(&view, "area_system", "selectable", "true");
    view_reset(&view);

}

void view_settings_setup(void)
{

    widget_area_init(view_createwidget(&view), "area_audio", WIDGET_IN_DEFAULT, 0, 0, 4, 3);
    widget_area_init(view_createwidget(&view), "area_audio_icon", WIDGET_IN_DEFAULT, 0, 0, 4, 2);
    widget_icon_init(view_createwidget(&view), WIDGET_ID_DEFAULT, "area_audio_icon", ICON_AUDIO);
    widget_area_init(view_createwidget(&view), "area_audio_text", WIDGET_IN_DEFAULT, 0, 2, 4, 1);
    widget_text_init(view_createwidget(&view), WIDGET_ID_DEFAULT, "area_audio_text", TEXT_TYPE_SELECT, TEXT_ALIGN_CENTER, "Audio");
    widget_area_init(view_createwidget(&view), "area_network", WIDGET_IN_DEFAULT, 0, 3, 4, 3);
    widget_area_init(view_createwidget(&view), "area_network_icon", WIDGET_IN_DEFAULT, 0, 3, 4, 2);
    widget_icon_init(view_createwidget(&view), WIDGET_ID_DEFAULT, "area_network_icon", ICON_NETWORK);
    widget_area_init(view_createwidget(&view), "area_network_text", WIDGET_IN_DEFAULT, 0, 5, 4, 1);
    widget_text_init(view_createwidget(&view), WIDGET_ID_DEFAULT, "area_network_text", TEXT_TYPE_SELECT, TEXT_ALIGN_CENTER, "Network");
    widget_area_init(view_createwidget(&view), "area_system", WIDGET_IN_DEFAULT, 4, 3, 4, 3);
    widget_area_init(view_createwidget(&view), "area_system_icon", WIDGET_IN_DEFAULT, 4, 3, 4, 2);
    widget_icon_init(view_createwidget(&view), WIDGET_ID_DEFAULT, "area_system_icon", ICON_SYSTEM);
    widget_area_init(view_createwidget(&view), "area_system_text", WIDGET_IN_DEFAULT, 4, 5, 4, 1);
    widget_text_init(view_createwidget(&view), WIDGET_ID_DEFAULT, "area_system_text", TEXT_TYPE_SELECT, TEXT_ALIGN_CENTER, "System");
    view_init(&view, "settings", load, 0, 0, button);
    main_registerview(&view);

}

