#include "include.h"

static struct view view;

static void button(unsigned int key)
{

    view_moveselection(&view, key);
    view_select(&view, key, "area_audio", view.name, "settings_audio");
    view_select(&view, key, "area_clock", view.name, "settings_datetime");
    view_select(&view, key, "area_system", view.name, "settings_system");
    view_unselect(&view, key, view.name);

}

static void load(unsigned int type)
{

    view_setattr(&view, "area_audio", "selectable", "true");
    view_setattr(&view, "area_clock", "selectable", "true");
    view_setattr(&view, "area_network", "selectable", "true");
    view_setattr(&view, "area_system", "selectable", "true");

}

void view_settings_setup(void)
{

    view_init(&view, "settings", load, 0, 0, button);
    widget_area_init(view_createwidget(&view), "area_audio", WIDGET_IN_DEFAULT, 0, 0, 4, 3);
    widget_area_init(view_createwidget(&view), "area_audio_icon", WIDGET_IN_DEFAULT, 0, 0, 4, 2);
    widget_area_init(view_createwidget(&view), "area_audio_text", WIDGET_IN_DEFAULT, 0, 2, 4, 1);
    widget_area_init(view_createwidget(&view), "area_clock", WIDGET_IN_DEFAULT, 8, 0, 4, 3);
    widget_area_init(view_createwidget(&view), "area_clock_icon", WIDGET_IN_DEFAULT, 8, 0, 4, 2);
    widget_area_init(view_createwidget(&view), "area_clock_text", WIDGET_IN_DEFAULT, 8, 2, 4, 1);
    widget_area_init(view_createwidget(&view), "area_network", WIDGET_IN_DEFAULT, 0, 3, 4, 3);
    widget_area_init(view_createwidget(&view), "area_network_icon", WIDGET_IN_DEFAULT, 0, 3, 4, 2);
    widget_area_init(view_createwidget(&view), "area_network_text", WIDGET_IN_DEFAULT, 0, 5, 4, 1);
    widget_area_init(view_createwidget(&view), "area_system", WIDGET_IN_DEFAULT, 4, 3, 4, 3);
    widget_area_init(view_createwidget(&view), "area_system_icon", WIDGET_IN_DEFAULT, 4, 3, 4, 2);
    widget_area_init(view_createwidget(&view), "area_system_text", WIDGET_IN_DEFAULT, 4, 5, 4, 1);
    widget_text_init(view_createwidget(&view), WIDGET_ID_DEFAULT, "area_audio_text", TEXT_TYPE_SELECT, TEXT_ALIGN_CENTER, "Audio");
    widget_text_init(view_createwidget(&view), WIDGET_ID_DEFAULT, "area_clock_text", TEXT_TYPE_SELECT, TEXT_ALIGN_CENTER, "Date&Time");
    widget_text_init(view_createwidget(&view), WIDGET_ID_DEFAULT, "area_network_text", TEXT_TYPE_DISABLE, TEXT_ALIGN_CENTER, "Network");
    widget_text_init(view_createwidget(&view), WIDGET_ID_DEFAULT, "area_system_text", TEXT_TYPE_SELECT, TEXT_ALIGN_CENTER, "System");
    widget_icon_init(view_createwidget(&view), WIDGET_ID_DEFAULT, "area_audio_icon", ICON_AUDIO);
    widget_icon_init(view_createwidget(&view), WIDGET_ID_DEFAULT, "area_clock_icon", ICON_CLOCK);
    widget_icon_init(view_createwidget(&view), WIDGET_ID_DEFAULT, "area_network_icon", ICON_NETWORK);
    widget_icon_init(view_createwidget(&view), WIDGET_ID_DEFAULT, "area_system_icon", ICON_SYSTEM);
    main_registerview(&view);

}

