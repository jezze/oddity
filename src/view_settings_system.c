#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "define.h"
#include "box.h"
#include "list.h"
#include "widget.h"
#include "view.h"
#include "main.h"
#include "session.h"
#include "helper.h"

static struct view view;
static char language[32];
static char timezone[32];
static char date[32];
static char time[32];

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

static void date_ondata(unsigned int id, void *data, unsigned int count)
{

    char *s = data;

    snprintf(date, 32, "%s", s);

}

static void time_ondata(unsigned int id, void *data, unsigned int count)
{

    char *s = data;

    snprintf(time, 32, "%s", s);

}

static void timezone_ondata(unsigned int id, void *data, unsigned int count)
{

    char *s = data;

    snprintf(timezone, 32, "%s", s);

}

static void step(unsigned int ticks)
{

    if (ticks % 60 == 0)
    {

        helper_date_get(2, date_ondata, 0, 0);
        helper_time_get(2, time_ondata, 0, 0);

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

static void load(void)
{

    snprintf(language, 32, "English (US)");
    snprintf(timezone, 32, "-");
    snprintf(date, 32, "-");
    view_setattr(&view, "text_autostart", "type", "disable");
    view_setattr(&view, "toggle_autostart", "state", "disabled");
    view_setattr(&view, "area_language", "selectable", "true");
    view_setattr(&view, "area_timezone", "selectable", "true");
    view_setattr(&view, "area_date", "selectable", "true");
    view_setattr(&view, "area_time", "selectable", "true");
    view_setattr(&view, "area_autostart", "selectable", "true");
    view_setattr(&view, "area_upgrade", "selectable", "true");
    view_setattr(&view, "area_sysinfo", "selectable", "true");
    view_reset(&view);
    helper_autostart_get(1, autostart_ondata, 0, 0);
    helper_date_get(1, date_ondata, 0, 0);
    helper_time_get(1, time_ondata, 0, 0);
    helper_timezone_get(1, timezone_ondata, 0, 0);

}

void view_settings_system_setup(void)
{

    widget_area_init(view_createwidget(&view), "area_language", WIDGET_IN_DEFAULT, 0, 0, 12, 1);
    widget_text_init(view_createwidget(&view), WIDGET_ID_DEFAULT, "area_language", TEXT_TYPE_SELECT, TEXT_ALIGN_LEFT, "Language");
    widget_text_init(view_createwidget(&view), "text_language", "area_language", TEXT_TYPE_NORMAL, TEXT_ALIGN_RIGHT, language);
    widget_area_init(view_createwidget(&view), "area_timezone", WIDGET_IN_DEFAULT, 0, 1, 12, 1);
    widget_text_init(view_createwidget(&view), WIDGET_ID_DEFAULT, "area_timezone", TEXT_TYPE_SELECT, TEXT_ALIGN_LEFT, "Timezone");
    widget_text_init(view_createwidget(&view), "text_timezone", "area_timezone", TEXT_TYPE_NORMAL, TEXT_ALIGN_RIGHT, timezone);
    widget_area_init(view_createwidget(&view), "area_date", WIDGET_IN_DEFAULT, 0, 2, 12, 1);
    widget_text_init(view_createwidget(&view), WIDGET_ID_DEFAULT, "area_date", TEXT_TYPE_SELECT, TEXT_ALIGN_LEFT, "Date");
    widget_text_init(view_createwidget(&view), "text_date", "area_date", TEXT_TYPE_NORMAL, TEXT_ALIGN_RIGHT, date);
    widget_area_init(view_createwidget(&view), "area_time", WIDGET_IN_DEFAULT, 0, 3, 12, 1);
    widget_text_init(view_createwidget(&view), WIDGET_ID_DEFAULT, "area_time", TEXT_TYPE_SELECT, TEXT_ALIGN_LEFT, "Time");
    widget_text_init(view_createwidget(&view), "text_time", "area_time", TEXT_TYPE_NORMAL, TEXT_ALIGN_RIGHT, time);
    widget_area_init(view_createwidget(&view), "area_autostart", WIDGET_IN_DEFAULT, 0, 4, 12, 1);
    widget_text_init(view_createwidget(&view), "text_autostart", "area_autostart", TEXT_TYPE_SELECT, TEXT_ALIGN_LEFT, "Autostart");
    widget_toggle_init(view_createwidget(&view), "toggle_autostart", "area_autostart", TOGGLE_OFF_DISABLED);
    widget_area_init(view_createwidget(&view), "area_upgrade", WIDGET_IN_DEFAULT, 0, 5, 12, 1);
    widget_text_init(view_createwidget(&view), WIDGET_ID_DEFAULT, "area_upgrade", TEXT_TYPE_SELECT, TEXT_ALIGN_LEFT, "Upgrade");
    widget_text_init(view_createwidget(&view), WIDGET_ID_DEFAULT, "area_upgrade", TEXT_TYPE_NORMAL, TEXT_ALIGN_RIGHT, "V1.02");
    widget_area_init(view_createwidget(&view), "area_sysinfo", WIDGET_IN_DEFAULT, 0, 6, 12, 1);
    widget_text_init(view_createwidget(&view), WIDGET_ID_DEFAULT, "area_sysinfo", TEXT_TYPE_SELECT, TEXT_ALIGN_LEFT, "System Information");
    view_init(&view, "settings_system", load, step, 0, button);
    main_registerview(&view);

}

