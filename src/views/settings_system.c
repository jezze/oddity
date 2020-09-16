#include "include.h"

static struct view view;
static char language[32];

static void autostart_ondata(unsigned int id, void *data, unsigned int count)
{

    if (!strncmp(data, "off", 3))
    {

        view_setattr(&view, "text_autostart", "type", "select");
        view_setattr(&view, "toggle_autostart", "state", "off");

    }

    else if (!strncmp(data, "on", 2))
    {

        view_setattr(&view, "text_autostart", "type", "select");
        view_setattr(&view, "toggle_autostart", "state", "on");

    }

    else
    {

        view_setattr(&view, "text_autostart", "type", "select");
        view_setattr(&view, "toggle_autostart", "state", "disabled");

    }

}

static void button(unsigned int key)
{

    view_moveselection(&view, key);
    view_select(&view, key, "area_sysinfo", view.name, "sysinfo");

    switch (key)
    {

    case KEY_A:
        if (view_isactive(&view, "area_autostart"))
        {

            struct widget *widget = view_findwidget(&view, "toggle_autostart");

            switch (widget->payload.toggle.state)
            {

            case TOGGLE_OFF:
                helper_autostart_install(1, autostart_ondata, 0, 0);

                break;

            case TOGGLE_ON:
                helper_autostart_uninstall(1, autostart_ondata, 0, 0);

                break;

            }

        }

        break;

    }

    view_unselect(&view, key, view.name);

}

static void load(unsigned int type)
{

    snprintf(language, 32, "English (US)");
    view_setattr(&view, "text_autostart", "type", "disable");
    view_setattr(&view, "toggle_autostart", "state", "disabled");
    view_setattr(&view, "area_language", "selectable", "true");
    view_setattr(&view, "area_autostart", "selectable", "true");
    view_setattr(&view, "area_upgrade", "selectable", "true");
    view_setattr(&view, "area_sysinfo", "selectable", "true");
    view_reset(&view);
    helper_autostart_get(1, autostart_ondata, 0, 0);

}

void view_settings_system_setup(void)
{

    view_init(&view, "settings_system", load, 0, 0, button);
    widget_area_init(view_createwidget(&view), "area_language", WIDGET_IN_DEFAULT, 0, 0, 12, 1);
    widget_area_init(view_createwidget(&view), "area_autostart", WIDGET_IN_DEFAULT, 0, 1, 12, 1);
    widget_area_init(view_createwidget(&view), "area_upgrade", WIDGET_IN_DEFAULT, 0, 2, 12, 1);
    widget_area_init(view_createwidget(&view), "area_sysinfo", WIDGET_IN_DEFAULT, 0, 3, 12, 1);
    widget_text_init(view_createwidget(&view), WIDGET_ID_DEFAULT, "area_language", TEXT_TYPE_SELECT, TEXT_ALIGN_LEFT, "Language");
    widget_text_init(view_createwidget(&view), "text_language", "area_language", TEXT_TYPE_NORMAL, TEXT_ALIGN_RIGHT, language);
    widget_text_init(view_createwidget(&view), "text_autostart", "area_autostart", TEXT_TYPE_SELECT, TEXT_ALIGN_LEFT, "Autostart");
    widget_text_init(view_createwidget(&view), WIDGET_ID_DEFAULT, "area_upgrade", TEXT_TYPE_SELECT, TEXT_ALIGN_LEFT, "Upgrade");
    widget_text_init(view_createwidget(&view), WIDGET_ID_DEFAULT, "area_upgrade", TEXT_TYPE_NORMAL, TEXT_ALIGN_RIGHT, "V1.02");
    widget_text_init(view_createwidget(&view), WIDGET_ID_DEFAULT, "area_sysinfo", TEXT_TYPE_SELECT, TEXT_ALIGN_LEFT, "System Information");
    widget_toggle_init(view_createwidget(&view), "toggle_autostart", "area_autostart", TOGGLE_OFF_DISABLED);
    main_registerview(&view);

}

