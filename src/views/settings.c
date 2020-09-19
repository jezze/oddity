#include "include.h"

static struct view view;

static void onbutton(unsigned int key)
{

    view_moveselection(&view, key);
    view_select(&view, key, "area_audio", "settings_audio");
    view_select(&view, key, "area_clock", "settings_datetime");
    view_select(&view, key, "area_system", "settings_system");
    view_unselect(&view, key);

}

static void onload(unsigned int type)
{

    view_setattr(&view, "area_audio", "selectable", "true");
    view_setattr(&view, "area_clock", "selectable", "true");
    view_setattr(&view, "area_network", "selectable", "true");
    view_setattr(&view, "area_system", "selectable", "true");

}

void view_settings_setup(void)
{

    view_init(&view, "settings", onload, 0, 0, onbutton);
    pool_area_create(&view, "area_audio", WIDGET_IN_DEFAULT, 0, 0, 4, 3);
    pool_area_create(&view, "area_audio_icon", WIDGET_IN_DEFAULT, 0, 0, 4, 2);
    pool_area_create(&view, "area_audio_text", WIDGET_IN_DEFAULT, 0, 2, 4, 1);
    pool_area_create(&view, "area_clock", WIDGET_IN_DEFAULT, 8, 0, 4, 3);
    pool_area_create(&view, "area_clock_icon", WIDGET_IN_DEFAULT, 8, 0, 4, 2);
    pool_area_create(&view, "area_clock_text", WIDGET_IN_DEFAULT, 8, 2, 4, 1);
    pool_area_create(&view, "area_network", WIDGET_IN_DEFAULT, 0, 3, 4, 3);
    pool_area_create(&view, "area_network_icon", WIDGET_IN_DEFAULT, 0, 3, 4, 2);
    pool_area_create(&view, "area_network_text", WIDGET_IN_DEFAULT, 0, 5, 4, 1);
    pool_area_create(&view, "area_system", WIDGET_IN_DEFAULT, 4, 3, 4, 3);
    pool_area_create(&view, "area_system_icon", WIDGET_IN_DEFAULT, 4, 3, 4, 2);
    pool_area_create(&view, "area_system_text", WIDGET_IN_DEFAULT, 4, 5, 4, 1);
    pool_text_create(&view, WIDGET_ID_DEFAULT, "area_audio_text", TEXT_TYPE_SELECT, TEXT_ALIGN_CENTER, "Audio");
    pool_text_create(&view, WIDGET_ID_DEFAULT, "area_clock_text", TEXT_TYPE_SELECT, TEXT_ALIGN_CENTER, "Date&Time");
    pool_text_create(&view, WIDGET_ID_DEFAULT, "area_network_text", TEXT_TYPE_DISABLE, TEXT_ALIGN_CENTER, "Network");
    pool_text_create(&view, WIDGET_ID_DEFAULT, "area_system_text", TEXT_TYPE_SELECT, TEXT_ALIGN_CENTER, "System");
    pool_icon_create(&view, WIDGET_ID_DEFAULT, "area_audio_icon", ICON_AUDIO);
    pool_icon_create(&view, WIDGET_ID_DEFAULT, "area_clock_icon", ICON_CLOCK);
    pool_icon_create(&view, WIDGET_ID_DEFAULT, "area_network_icon", ICON_NETWORK);
    pool_icon_create(&view, WIDGET_ID_DEFAULT, "area_system_icon", ICON_SYSTEM);
    main_registerview(&view);

}

