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
    view_setattr(&view, "option_year_2020", "hidden", "false");
    view_setattr(&view, "option_month_01", "hidden", "false");
    view_setattr(&view, "option_day_01", "hidden", "false");
    view_reset(&view);

}

void view_settings_date_setup(void)
{

    view_init(&view, "settings_date", load, 0, 0, button);
    widget_area_init(view_createwidget(&view), "area_date", WIDGET_IN_DEFAULT, 0, 0, 12, 1);
    widget_area_init(view_createwidget(&view), "area_year", WIDGET_IN_DEFAULT, 0, 1, 12, 1);
    widget_area_init(view_createwidget(&view), "area_year_text", WIDGET_IN_DEFAULT, 0, 1, 6, 1);
    widget_area_init(view_createwidget(&view), "area_year_select", WIDGET_IN_DEFAULT, 6, 1, 6, 1);
    widget_area_init(view_createwidget(&view), "area_month", WIDGET_IN_DEFAULT, 0, 2, 12, 1);
    widget_area_init(view_createwidget(&view), "area_month_text", WIDGET_IN_DEFAULT, 0, 2, 6, 1);
    widget_area_init(view_createwidget(&view), "area_month_select", WIDGET_IN_DEFAULT, 6, 2, 6, 1);
    widget_area_init(view_createwidget(&view), "area_day", WIDGET_IN_DEFAULT, 0, 3, 12, 1);
    widget_area_init(view_createwidget(&view), "area_day_text", WIDGET_IN_DEFAULT, 0, 3, 6, 1);
    widget_area_init(view_createwidget(&view), "area_day_select", WIDGET_IN_DEFAULT, 6, 3, 6, 1);
    widget_text_init(view_createwidget(&view), WIDGET_ID_DEFAULT, "area_date", TEXT_TYPE_TITLE, TEXT_ALIGN_CENTER, "Date");
    widget_text_init(view_createwidget(&view), "text_year", "area_year_text", TEXT_TYPE_NORMAL, TEXT_ALIGN_LEFT, "Year");
    widget_text_init(view_createwidget(&view), "text_month", "area_month_text", TEXT_TYPE_NORMAL, TEXT_ALIGN_LEFT, "Month");
    widget_text_init(view_createwidget(&view), "text_day", "area_day_text", TEXT_TYPE_NORMAL, TEXT_ALIGN_LEFT, "Day");
    widget_select_init(view_createwidget(&view), "select_year", "area_year_select");
    widget_select_init(view_createwidget(&view), "select_month", "area_month_select");
    widget_select_init(view_createwidget(&view), "select_day", "area_day_select");
    widget_option_init(view_createwidget(&view), "option_year_1970", "select_year", "1970", "1970");
    widget_option_init(view_createwidget(&view), "option_year_1971", "select_year", "1971", "1971");
    widget_option_init(view_createwidget(&view), "option_year_1972", "select_year", "1972", "1972");
    widget_option_init(view_createwidget(&view), "option_year_1973", "select_year", "1973", "1973");
    widget_option_init(view_createwidget(&view), "option_year_1974", "select_year", "1974", "1974");
    widget_option_init(view_createwidget(&view), "option_year_1975", "select_year", "1975", "1975");
    widget_option_init(view_createwidget(&view), "option_year_1976", "select_year", "1976", "1976");
    widget_option_init(view_createwidget(&view), "option_year_1977", "select_year", "1977", "1977");
    widget_option_init(view_createwidget(&view), "option_year_1978", "select_year", "1978", "1978");
    widget_option_init(view_createwidget(&view), "option_year_1979", "select_year", "1979", "1979");
    widget_option_init(view_createwidget(&view), "option_year_1980", "select_year", "1980", "1980");
    widget_option_init(view_createwidget(&view), "option_year_1981", "select_year", "1981", "1981");
    widget_option_init(view_createwidget(&view), "option_year_1982", "select_year", "1982", "1982");
    widget_option_init(view_createwidget(&view), "option_year_1983", "select_year", "1983", "1983");
    widget_option_init(view_createwidget(&view), "option_year_1984", "select_year", "1984", "1984");
    widget_option_init(view_createwidget(&view), "option_year_1985", "select_year", "1985", "1985");
    widget_option_init(view_createwidget(&view), "option_year_1986", "select_year", "1986", "1986");
    widget_option_init(view_createwidget(&view), "option_year_1987", "select_year", "1987", "1987");
    widget_option_init(view_createwidget(&view), "option_year_1988", "select_year", "1988", "1988");
    widget_option_init(view_createwidget(&view), "option_year_1989", "select_year", "1989", "1989");
    widget_option_init(view_createwidget(&view), "option_year_1990", "select_year", "1990", "1990");
    widget_option_init(view_createwidget(&view), "option_year_1991", "select_year", "1991", "1991");
    widget_option_init(view_createwidget(&view), "option_year_1992", "select_year", "1992", "1992");
    widget_option_init(view_createwidget(&view), "option_year_1993", "select_year", "1993", "1993");
    widget_option_init(view_createwidget(&view), "option_year_1994", "select_year", "1994", "1994");
    widget_option_init(view_createwidget(&view), "option_year_1995", "select_year", "1995", "1995");
    widget_option_init(view_createwidget(&view), "option_year_1996", "select_year", "1996", "1996");
    widget_option_init(view_createwidget(&view), "option_year_1997", "select_year", "1997", "1997");
    widget_option_init(view_createwidget(&view), "option_year_1998", "select_year", "1998", "1998");
    widget_option_init(view_createwidget(&view), "option_year_1999", "select_year", "1999", "1999");
    widget_option_init(view_createwidget(&view), "option_year_2000", "select_year", "2000", "2000");
    widget_option_init(view_createwidget(&view), "option_year_2001", "select_year", "2001", "2001");
    widget_option_init(view_createwidget(&view), "option_year_2002", "select_year", "2002", "2002");
    widget_option_init(view_createwidget(&view), "option_year_2003", "select_year", "2003", "2003");
    widget_option_init(view_createwidget(&view), "option_year_2004", "select_year", "2004", "2004");
    widget_option_init(view_createwidget(&view), "option_year_2005", "select_year", "2005", "2005");
    widget_option_init(view_createwidget(&view), "option_year_2006", "select_year", "2006", "2006");
    widget_option_init(view_createwidget(&view), "option_year_2007", "select_year", "2007", "2007");
    widget_option_init(view_createwidget(&view), "option_year_2008", "select_year", "2008", "2008");
    widget_option_init(view_createwidget(&view), "option_year_2009", "select_year", "2009", "2009");
    widget_option_init(view_createwidget(&view), "option_year_2010", "select_year", "2010", "2010");
    widget_option_init(view_createwidget(&view), "option_year_2011", "select_year", "2011", "2011");
    widget_option_init(view_createwidget(&view), "option_year_2012", "select_year", "2012", "2012");
    widget_option_init(view_createwidget(&view), "option_year_2013", "select_year", "2013", "2013");
    widget_option_init(view_createwidget(&view), "option_year_2014", "select_year", "2014", "2014");
    widget_option_init(view_createwidget(&view), "option_year_2015", "select_year", "2015", "2015");
    widget_option_init(view_createwidget(&view), "option_year_2016", "select_year", "2016", "2016");
    widget_option_init(view_createwidget(&view), "option_year_2017", "select_year", "2017", "2017");
    widget_option_init(view_createwidget(&view), "option_year_2018", "select_year", "2018", "2018");
    widget_option_init(view_createwidget(&view), "option_year_2019", "select_year", "2019", "2019");
    widget_option_init(view_createwidget(&view), "option_year_2020", "select_year", "2020", "2020");
    widget_option_init(view_createwidget(&view), "option_year_2021", "select_year", "2021", "2021");
    widget_option_init(view_createwidget(&view), "option_year_2022", "select_year", "2022", "2022");
    widget_option_init(view_createwidget(&view), "option_year_2023", "select_year", "2023", "2023");
    widget_option_init(view_createwidget(&view), "option_year_2024", "select_year", "2024", "2024");
    widget_option_init(view_createwidget(&view), "option_year_2025", "select_year", "2025", "2025");
    widget_option_init(view_createwidget(&view), "option_year_2026", "select_year", "2026", "2026");
    widget_option_init(view_createwidget(&view), "option_year_2027", "select_year", "2027", "2027");
    widget_option_init(view_createwidget(&view), "option_year_2028", "select_year", "2028", "2028");
    widget_option_init(view_createwidget(&view), "option_year_2029", "select_year", "2029", "2029");
    widget_option_init(view_createwidget(&view), "option_year_2030", "select_year", "2030", "2030");
    widget_option_init(view_createwidget(&view), "option_month_01", "select_month", "01", "01");
    widget_option_init(view_createwidget(&view), "option_month_02", "select_month", "02", "02");
    widget_option_init(view_createwidget(&view), "option_month_03", "select_month", "03", "03");
    widget_option_init(view_createwidget(&view), "option_month_04", "select_month", "04", "04");
    widget_option_init(view_createwidget(&view), "option_month_05", "select_month", "05", "05");
    widget_option_init(view_createwidget(&view), "option_month_06", "select_month", "06", "06");
    widget_option_init(view_createwidget(&view), "option_month_07", "select_month", "07", "07");
    widget_option_init(view_createwidget(&view), "option_month_08", "select_month", "08", "08");
    widget_option_init(view_createwidget(&view), "option_month_09", "select_month", "09", "09");
    widget_option_init(view_createwidget(&view), "option_month_10", "select_month", "10", "10");
    widget_option_init(view_createwidget(&view), "option_month_11", "select_month", "11", "11");
    widget_option_init(view_createwidget(&view), "option_month_12", "select_month", "12", "12");
    widget_option_init(view_createwidget(&view), "option_day_01", "select_day", "01", "01");
    widget_option_init(view_createwidget(&view), "option_day_02", "select_day", "02", "02");
    widget_option_init(view_createwidget(&view), "option_day_03", "select_day", "03", "03");
    widget_option_init(view_createwidget(&view), "option_day_04", "select_day", "04", "04");
    widget_option_init(view_createwidget(&view), "option_day_05", "select_day", "05", "05");
    widget_option_init(view_createwidget(&view), "option_day_06", "select_day", "06", "06");
    widget_option_init(view_createwidget(&view), "option_day_07", "select_day", "07", "07");
    widget_option_init(view_createwidget(&view), "option_day_08", "select_day", "08", "08");
    widget_option_init(view_createwidget(&view), "option_day_09", "select_day", "09", "09");
    widget_option_init(view_createwidget(&view), "option_day_10", "select_day", "10", "10");
    widget_option_init(view_createwidget(&view), "option_day_11", "select_day", "11", "11");
    widget_option_init(view_createwidget(&view), "option_day_12", "select_day", "12", "12");
    widget_option_init(view_createwidget(&view), "option_day_13", "select_day", "13", "13");
    widget_option_init(view_createwidget(&view), "option_day_14", "select_day", "14", "14");
    widget_option_init(view_createwidget(&view), "option_day_15", "select_day", "15", "15");
    widget_option_init(view_createwidget(&view), "option_day_16", "select_day", "16", "16");
    widget_option_init(view_createwidget(&view), "option_day_17", "select_day", "17", "17");
    widget_option_init(view_createwidget(&view), "option_day_18", "select_day", "18", "18");
    widget_option_init(view_createwidget(&view), "option_day_19", "select_day", "19", "19");
    widget_option_init(view_createwidget(&view), "option_day_20", "select_day", "20", "20");
    widget_option_init(view_createwidget(&view), "option_day_21", "select_day", "21", "21");
    widget_option_init(view_createwidget(&view), "option_day_22", "select_day", "22", "22");
    widget_option_init(view_createwidget(&view), "option_day_23", "select_day", "23", "23");
    widget_option_init(view_createwidget(&view), "option_day_24", "select_day", "24", "24");
    widget_option_init(view_createwidget(&view), "option_day_25", "select_day", "25", "25");
    widget_option_init(view_createwidget(&view), "option_day_26", "select_day", "26", "26");
    widget_option_init(view_createwidget(&view), "option_day_27", "select_day", "27", "27");
    widget_option_init(view_createwidget(&view), "option_day_28", "select_day", "28", "28");
    widget_option_init(view_createwidget(&view), "option_day_29", "select_day", "29", "29");
    widget_option_init(view_createwidget(&view), "option_day_30", "select_day", "30", "30");
    widget_option_init(view_createwidget(&view), "option_day_31", "select_day", "31", "31");
    main_registerview(&view);

}

