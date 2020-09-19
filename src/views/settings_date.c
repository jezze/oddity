#include "include.h"

static struct view view;
static char year[8];
static char month[8];
static char day[8];

static void date_ondata(unsigned int id, void *data, unsigned int count)
{

    if (count >= 10)
    {

        char *s = data;

        memcpy(year, s, 4);
        memcpy(month, s + 5, 2);
        memcpy(day, s + 8, 2);

    }

}

static void onbutton(unsigned int key)
{

    view_moveselection(&view, key);

    switch (key)
    {

    case KEY_A:
        if (view_isactive(&view, "area_save"))
        {

            /* Call helper here */
            view_unselect(&view, KEY_B);

        }

        break;

    case KEY_LEFT:
        if (view_isactive(&view, "area_year"))
            view_goprev(&view, key, "select_year");

        if (view_isactive(&view, "area_month"))
            view_goprev(&view, key, "select_month");

        if (view_isactive(&view, "area_day"))
            view_goprev(&view, key, "select_day");

        break;

    case KEY_RIGHT:
        if (view_isactive(&view, "area_year"))
            view_gonext(&view, key, "select_year");

        if (view_isactive(&view, "area_month"))
            view_gonext(&view, key, "select_month");

        if (view_isactive(&view, "area_day"))
            view_gonext(&view, key, "select_day");

        break;

    }

    view_unselect(&view, key);

}

static void onload(unsigned int type)
{

    view_setattr(&view, "area_year", "selectable", "true");
    view_setattr(&view, "area_month", "selectable", "true");
    view_setattr(&view, "area_day", "selectable", "true");
    view_setattr(&view, "area_save", "selectable", "true");
    helper_date_get(1, date_ondata, 0, 0);

}

void view_settings_date_setup(void)
{

    view_init(&view, "settings_date", onload, 0, 0, onbutton);
    pool_create_area(&view, "area_year", WIDGET_IN_DEFAULT, 2, 2, 8, 1);
    pool_create_area(&view, "area_year_text", WIDGET_IN_DEFAULT, 2, 2, 4, 1);
    pool_create_area(&view, "area_year_select", WIDGET_IN_DEFAULT, 6, 2, 4, 1);
    pool_create_area(&view, "area_month", WIDGET_IN_DEFAULT, 2, 3, 8, 1);
    pool_create_area(&view, "area_month_text", WIDGET_IN_DEFAULT, 2, 3, 4, 1);
    pool_create_area(&view, "area_month_select", WIDGET_IN_DEFAULT, 6, 3, 4, 1);
    pool_create_area(&view, "area_day", WIDGET_IN_DEFAULT, 2, 4, 8, 1);
    pool_create_area(&view, "area_day_text", WIDGET_IN_DEFAULT, 2, 4, 4, 1);
    pool_create_area(&view, "area_day_select", WIDGET_IN_DEFAULT, 6, 4, 4, 1);
    pool_create_area(&view, "area_save", WIDGET_IN_DEFAULT, 2, 6, 8, 1);
    pool_create_text(&view, WIDGET_ID_DEFAULT, "area_year_text", TEXT_TYPE_NORMAL, TEXT_ALIGN_LEFT, "Year");
    pool_create_text(&view, WIDGET_ID_DEFAULT, "area_month_text", TEXT_TYPE_NORMAL, TEXT_ALIGN_LEFT, "Month");
    pool_create_text(&view, WIDGET_ID_DEFAULT, "area_day_text", TEXT_TYPE_NORMAL, TEXT_ALIGN_LEFT, "Day");
    pool_create_text(&view, WIDGET_ID_DEFAULT, "area_save", TEXT_TYPE_SELECT, TEXT_ALIGN_CENTER, "Save");
    pool_create_select(&view, "select_year", "area_year_select", year);
    pool_create_select(&view, "select_month", "area_month_select", month);
    pool_create_select(&view, "select_day", "area_day_select", day);
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_year", "1970", "1970");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_year", "1971", "1971");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_year", "1972", "1972");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_year", "1973", "1973");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_year", "1974", "1974");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_year", "1975", "1975");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_year", "1976", "1976");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_year", "1977", "1977");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_year", "1978", "1978");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_year", "1979", "1979");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_year", "1980", "1980");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_year", "1981", "1981");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_year", "1982", "1982");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_year", "1983", "1983");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_year", "1984", "1984");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_year", "1985", "1985");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_year", "1986", "1986");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_year", "1987", "1987");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_year", "1988", "1988");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_year", "1989", "1989");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_year", "1990", "1990");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_year", "1991", "1991");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_year", "1992", "1992");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_year", "1993", "1993");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_year", "1994", "1994");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_year", "1995", "1995");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_year", "1996", "1996");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_year", "1997", "1997");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_year", "1998", "1998");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_year", "1999", "1999");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_year", "2000", "2000");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_year", "2001", "2001");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_year", "2002", "2002");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_year", "2003", "2003");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_year", "2004", "2004");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_year", "2005", "2005");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_year", "2006", "2006");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_year", "2007", "2007");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_year", "2008", "2008");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_year", "2009", "2009");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_year", "2010", "2010");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_year", "2011", "2011");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_year", "2012", "2012");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_year", "2013", "2013");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_year", "2014", "2014");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_year", "2015", "2015");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_year", "2016", "2016");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_year", "2017", "2017");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_year", "2018", "2018");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_year", "2019", "2019");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_year", "2020", "2020");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_year", "2021", "2021");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_year", "2022", "2022");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_year", "2023", "2023");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_year", "2024", "2024");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_year", "2025", "2025");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_year", "2026", "2026");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_year", "2027", "2027");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_year", "2028", "2028");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_year", "2029", "2029");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_year", "2030", "2030");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_month", "01", "01");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_month", "02", "02");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_month", "03", "03");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_month", "04", "04");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_month", "05", "05");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_month", "06", "06");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_month", "07", "07");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_month", "08", "08");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_month", "09", "09");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_month", "10", "10");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_month", "11", "11");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_month", "12", "12");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_day", "01", "01");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_day", "02", "02");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_day", "03", "03");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_day", "04", "04");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_day", "05", "05");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_day", "06", "06");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_day", "07", "07");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_day", "08", "08");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_day", "09", "09");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_day", "10", "10");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_day", "11", "11");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_day", "12", "12");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_day", "13", "13");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_day", "14", "14");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_day", "15", "15");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_day", "16", "16");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_day", "17", "17");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_day", "18", "18");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_day", "19", "19");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_day", "20", "20");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_day", "21", "21");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_day", "22", "22");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_day", "23", "23");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_day", "24", "24");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_day", "25", "25");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_day", "26", "26");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_day", "27", "27");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_day", "28", "28");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_day", "29", "29");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_day", "30", "30");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_day", "31", "31");
    main_registerview(&view);

}

