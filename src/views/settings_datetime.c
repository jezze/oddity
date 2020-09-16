#include "include.h"

static struct view view;
static char timezone[32];
static char date[32];
static char time[32];

static void timezone_ondata(unsigned int id, void *data, unsigned int count)
{

    char *s = data;

    snprintf(timezone, 32, "%s", s);

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
    view_select(&view, key, "area_date", view.name, "settings_date");
    view_unselect(&view, key, view.name);

}

static void load(unsigned int type)
{

    snprintf(timezone, 32, "-");
    snprintf(date, 32, "-");
    snprintf(time, 32, "-");
    view_setattr(&view, "area_timezone", "selectable", "true");
    view_setattr(&view, "area_date", "selectable", "true");
    view_setattr(&view, "area_time", "selectable", "true");
    view_reset(&view);
    helper_timezone_get(1, timezone_ondata, 0, 0);
    helper_date_get(1, date_ondata, 0, 0);
    helper_time_get(1, time_ondata, 0, 0);

}

void view_settings_datetime_setup(void)
{

    view_init(&view, "settings_datetime", load, step, 0, button);
    widget_area_init(view_createwidget(&view), "area_timezone", WIDGET_IN_DEFAULT, 0, 0, 12, 1);
    widget_area_init(view_createwidget(&view), "area_date", WIDGET_IN_DEFAULT, 0, 1, 12, 1);
    widget_area_init(view_createwidget(&view), "area_time", WIDGET_IN_DEFAULT, 0, 2, 12, 1);
    widget_text_init(view_createwidget(&view), WIDGET_ID_DEFAULT, "area_timezone", TEXT_TYPE_SELECT, TEXT_ALIGN_LEFT, "Timezone");
    widget_text_init(view_createwidget(&view), "text_timezone", "area_timezone", TEXT_TYPE_NORMAL, TEXT_ALIGN_RIGHT, timezone);
    widget_text_init(view_createwidget(&view), WIDGET_ID_DEFAULT, "area_date", TEXT_TYPE_SELECT, TEXT_ALIGN_LEFT, "Date");
    widget_text_init(view_createwidget(&view), "text_date", "area_date", TEXT_TYPE_NORMAL, TEXT_ALIGN_RIGHT, date);
    widget_text_init(view_createwidget(&view), WIDGET_ID_DEFAULT, "area_time", TEXT_TYPE_SELECT, TEXT_ALIGN_LEFT, "Time");
    widget_text_init(view_createwidget(&view), "text_time", "area_time", TEXT_TYPE_NORMAL, TEXT_ALIGN_RIGHT, time);
    main_registerview(&view);

}

