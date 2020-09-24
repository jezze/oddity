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

static void onbutton(unsigned int button)
{

    if (button == BUTTON_A)
    {

        if (view_isactive(&view, "area_sysinfo"))
            main_loadview(&view, "area_sysinfo", "sysinfo");

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

    }

}

static void onload(unsigned int type)
{

    snprintf(language, 32, "English (US)");
    view_setattr(&view, "text_autostart", "type", "disable");
    view_setattr(&view, "toggle_autostart", "state", "disabled");
    view_setattr(&view, "area_language", "selectable", "true");
    view_setattr(&view, "area_autostart", "selectable", "true");
    view_setattr(&view, "area_upgrade", "selectable", "true");
    view_setattr(&view, "area_sysinfo", "selectable", "true");
    helper_autostart_get(1, autostart_ondata, 0, 0);

}

void view_settings_system_setup(void)
{

    view_init(&view, "settings_system", onload, 0, 0, onbutton);
    pool_create_area(&view, "area_language", WIDGET_IN_DEFAULT, 0, 0, 12, 1);
    pool_create_area(&view, "area_autostart", WIDGET_IN_DEFAULT, 0, 1, 12, 1);
    pool_create_area(&view, "area_upgrade", WIDGET_IN_DEFAULT, 0, 2, 12, 1);
    pool_create_area(&view, "area_sysinfo", WIDGET_IN_DEFAULT, 0, 3, 12, 1);
    pool_create_text(&view, WIDGET_ID_DEFAULT, "area_language", TEXT_TYPE_SELECT, TEXT_ALIGN_LEFT, "Language");
    pool_create_text(&view, "text_language", "area_language", TEXT_TYPE_NORMAL, TEXT_ALIGN_RIGHT, language);
    pool_create_text(&view, "text_autostart", "area_autostart", TEXT_TYPE_SELECT, TEXT_ALIGN_LEFT, "Autostart");
    pool_create_text(&view, WIDGET_ID_DEFAULT, "area_upgrade", TEXT_TYPE_SELECT, TEXT_ALIGN_LEFT, "Upgrade");
    pool_create_text(&view, WIDGET_ID_DEFAULT, "area_upgrade", TEXT_TYPE_NORMAL, TEXT_ALIGN_RIGHT, "V1.02");
    pool_create_text(&view, WIDGET_ID_DEFAULT, "area_sysinfo", TEXT_TYPE_SELECT, TEXT_ALIGN_LEFT, "System Information");
    pool_create_toggle(&view, "toggle_autostart", "area_autostart", TOGGLE_OFF_DISABLED);
    main_registerview(&view);

}

