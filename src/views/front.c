#include "include.h"

static struct view view;

static void onbutton(unsigned int button)
{

    if (button == BUTTON_A)
    {

        if (view_isactive(&view, "area_library"))
            main_loadview(&view, "area_library", "repolist");

        if (view_isactive(&view, "area_settings"))
            main_loadview(&view, "area_settings", "settings");

        if (view_isactive(&view, "area_exit"))
            main_loadview(&view, "area_exit", "exit");

    }

}

static void onload(unsigned int type)
{

    view_setattr(&view, "area_library", "selectable", "true");
    view_setattr(&view, "area_settings", "selectable", "true");
    view_setattr(&view, "area_exit", "selectable", "true");

}

void view_front_setup(void)
{

    view_init(&view, "front", onload, 0, 0, onbutton);
    pool_create_area(&view, "area_library", WIDGET_IN_DEFAULT, 2, 3, 8, 1);
    pool_create_area(&view, "area_settings", WIDGET_IN_DEFAULT, 2, 4, 8, 1);
    pool_create_area(&view, "area_exit", WIDGET_IN_DEFAULT, 2, 5, 8, 1);
    pool_create_text(&view, WIDGET_ID_DEFAULT, "area_library", TEXT_TYPE_SELECT, TEXT_ALIGN_CENTER, "Library");
    pool_create_text(&view, WIDGET_ID_DEFAULT, "area_settings", TEXT_TYPE_SELECT, TEXT_ALIGN_CENTER, "Settings");
    pool_create_text(&view, WIDGET_ID_DEFAULT, "area_exit", TEXT_TYPE_SELECT, TEXT_ALIGN_CENTER, "Exit");
    main_registerview(&view);

}

