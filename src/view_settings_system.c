#include <stdlib.h>
#include <string.h>
#include "define.h"
#include "box.h"
#include "list.h"
#include "widget.h"
#include "view.h"
#include "main.h"
#include "session.h"

static struct view view;

static void ondata(unsigned int id, void *data, unsigned int count)
{

    if (!strncmp(data, "off", 3))
    {

        view_findwidget(&view, "text_autostart")->payload.text.color = TEXT_COLOR_SELECT;
        view_findwidget(&view, "toggle_autostart")->payload.toggle.state = TOGGLE_OFF;

    }

    else if (!strncmp(data, "on", 2))
    {

        view_findwidget(&view, "text_autostart")->payload.text.color = TEXT_COLOR_SELECT;
        view_findwidget(&view, "toggle_autostart")->payload.toggle.state = TOGGLE_ON;

    }

    else
    {

        view_findwidget(&view, "text_autostart")->payload.text.color = TEXT_COLOR_DISABLE;
        view_findwidget(&view, "toggle_autostart")->payload.toggle.state = TOGGLE_OFF_DISABLED;

    }

}

static void button(unsigned int key)
{

    view_moveselection(&view, key);

    switch (key)
    {

    case KEY_A:
        if (view_isactive(&view, "area_autostart"))
        {

            struct widget *widget = view_findwidget(&view, "toggle_autostart");

            switch (widget->payload.toggle.state)
            {

            case TOGGLE_OFF:
                session_create("settings_autostart_install", 1, ondata, 0, 0);
                session_setarg("settings_autostart_install", 1, 0, "./helper.sh");
                session_setarg("settings_autostart_install", 1, 1, "autostart_install");
                session_setarg("settings_autostart_install", 1, 2, 0);
                session_run();

                break;

            case TOGGLE_ON:
                session_create("settings_autostart_uninstall", 1, ondata, 0, 0);
                session_setarg("settings_autostart_uninstall", 1, 0, "./helper.sh");
                session_setarg("settings_autostart_uninstall", 1, 1, "autostart_uninstall");
                session_setarg("settings_autostart_uninstall", 1, 2, 0);
                session_run();

            }

        }

        break;

    }

    view_unselect(&view, key, view.name);

}

static void load(void)
{

    view_findwidget(&view, "text_autostart")->payload.text.color = TEXT_COLOR_DISABLE;
    view_findwidget(&view, "toggle_autostart")->payload.toggle.state = TOGGLE_OFF_DISABLED;
    view_findwidget(&view, "area_language")->selectable = 1;
    view_findwidget(&view, "area_region")->selectable = 1;
    view_findwidget(&view, "area_datetime")->selectable = 1;
    view_findwidget(&view, "area_autostart")->selectable = 1;
    view_findwidget(&view, "area_sysinfo")->selectable = 1;
    view_findwidget(&view, "area_upgrade")->selectable = 1;

    session_create("settings_autostart_get", 1, ondata, 0, 0);
    session_setarg("settings_autostart_get", 1, 0, "./helper.sh");
    session_setarg("settings_autostart_get", 1, 1, "autostart_get");
    session_setarg("settings_autostart_get", 1, 2, 0);
    session_run();
    view_reset(&view);

}

void view_settings_system_setup(void)
{

    widget_area_init(view_createwidget(&view), "area_language", WIDGET_IN_DEFAULT, 0, 0, 12, 1);
    widget_text_init(view_createwidget(&view), WIDGET_ID_DEFAULT, "area_language", TEXT_COLOR_SELECT, TEXT_ALIGN_LEFT, "Language");
    widget_text_init(view_createwidget(&view), WIDGET_ID_DEFAULT, "area_language", TEXT_COLOR_NORMAL, TEXT_ALIGN_RIGHT, "English (US)");
    widget_area_init(view_createwidget(&view), "area_region", WIDGET_IN_DEFAULT, 0, 1, 12, 1);
    widget_text_init(view_createwidget(&view), WIDGET_ID_DEFAULT, "area_region", TEXT_COLOR_SELECT, TEXT_ALIGN_LEFT, "Region");
    widget_text_init(view_createwidget(&view), WIDGET_ID_DEFAULT, "area_region", TEXT_COLOR_NORMAL, TEXT_ALIGN_RIGHT, "Sweden");
    widget_area_init(view_createwidget(&view), "area_datetime", WIDGET_IN_DEFAULT, 0, 2, 12, 1);
    widget_text_init(view_createwidget(&view), WIDGET_ID_DEFAULT, "area_datetime", TEXT_COLOR_SELECT, TEXT_ALIGN_LEFT, "Date and Time");
    widget_text_init(view_createwidget(&view), WIDGET_ID_DEFAULT, "area_datetime", TEXT_COLOR_NORMAL, TEXT_ALIGN_RIGHT, "2020-08-20 15:32:10");
    widget_area_init(view_createwidget(&view), "area_autostart", WIDGET_IN_DEFAULT, 0, 3, 12, 1);
    widget_text_init(view_createwidget(&view), "text_autostart", "area_autostart", TEXT_COLOR_SELECT, TEXT_ALIGN_LEFT, "Autostart");
    widget_toggle_init(view_createwidget(&view), "toggle_autostart", "area_autostart", TOGGLE_OFF_DISABLED);
    widget_area_init(view_createwidget(&view), "area_sysinfo", WIDGET_IN_DEFAULT, 0, 4, 12, 1);
    widget_text_init(view_createwidget(&view), WIDGET_ID_DEFAULT, "area_sysinfo", TEXT_COLOR_SELECT, TEXT_ALIGN_LEFT, "System Information");
    widget_area_init(view_createwidget(&view), "area_upgrade", WIDGET_IN_DEFAULT, 0, 5, 12, 1);
    widget_text_init(view_createwidget(&view), WIDGET_ID_DEFAULT, "area_upgrade", TEXT_COLOR_SELECT, TEXT_ALIGN_LEFT, "Upgrade");
    widget_text_init(view_createwidget(&view), WIDGET_ID_DEFAULT, "area_upgrade", TEXT_COLOR_NORMAL, TEXT_ALIGN_RIGHT, "V1.02");
    view_init(&view, "settings_system", load, 0, 0, button);
    main_registerview(&view);

}

