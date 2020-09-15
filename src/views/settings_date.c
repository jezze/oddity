#include "include.h"

static struct view view;

static void button(unsigned int key)
{

    view_moveselection(&view, key);

    switch (key)
    {

    case KEY_LEFT:
        if (view_isactive(&view, "area_year"))
            ;

        if (view_isactive(&view, "area_month"))
            ;

        if (view_isactive(&view, "area_day"))
            ;

        break;

    case KEY_RIGHT:
        if (view_isactive(&view, "area_year"))
            ;

        if (view_isactive(&view, "area_month"))
            ;

        if (view_isactive(&view, "area_day"))
            ;

        break;

    }

    view_unselect(&view, key, view.name);

}

static void load(void)
{

    view_setattr(&view, "area_year", "selectable", "true");
    view_setattr(&view, "area_month", "selectable", "true");
    view_setattr(&view, "area_day", "selectable", "true");
    view_reset(&view);

}

void view_settings_date_setup(void)
{

    view_init(&view, "settings_date", load, 0, 0, button);
    widget_area_init(view_createwidget(&view), "area_date", WIDGET_IN_DEFAULT, 0, 0, 12, 1);
    widget_area_init(view_createwidget(&view), "area_year", WIDGET_IN_DEFAULT, 0, 1, 12, 1);
    widget_area_init(view_createwidget(&view), "area_year_text", WIDGET_IN_DEFAULT, 0, 1, 8, 1);
    widget_area_init(view_createwidget(&view), "area_year_slider", WIDGET_IN_DEFAULT, 8, 1, 4, 1);
    widget_area_init(view_createwidget(&view), "area_month", WIDGET_IN_DEFAULT, 0, 2, 12, 1);
    widget_area_init(view_createwidget(&view), "area_month_text", WIDGET_IN_DEFAULT, 0, 2, 8, 1);
    widget_area_init(view_createwidget(&view), "area_month_slider", WIDGET_IN_DEFAULT, 8, 2, 4, 1);
    widget_area_init(view_createwidget(&view), "area_day", WIDGET_IN_DEFAULT, 0, 3, 12, 1);
    widget_area_init(view_createwidget(&view), "area_day_text", WIDGET_IN_DEFAULT, 0, 3, 8, 1);
    widget_area_init(view_createwidget(&view), "area_day_slider", WIDGET_IN_DEFAULT, 8, 3, 4, 1);
    widget_text_init(view_createwidget(&view), WIDGET_ID_DEFAULT, "area_date", TEXT_TYPE_TITLE, TEXT_ALIGN_CENTER, "Date");
    widget_text_init(view_createwidget(&view), "text_year", "area_year_text", TEXT_TYPE_NORMAL, TEXT_ALIGN_LEFT, "Year");
    widget_text_init(view_createwidget(&view), "text_month", "area_month_text", TEXT_TYPE_NORMAL, TEXT_ALIGN_LEFT, "Month");
    widget_text_init(view_createwidget(&view), "text_day", "area_day_text", TEXT_TYPE_NORMAL, TEXT_ALIGN_LEFT, "Day");
    widget_slider_init(view_createwidget(&view), "slider_year", "area_year_slider", 0, 100, -1);
    widget_slider_init(view_createwidget(&view), "slider_month", "area_month_slider", 0, 100, -1);
    widget_slider_init(view_createwidget(&view), "slider_day", "area_day_slider", 0, 100, -1);
    main_registerview(&view);

}

