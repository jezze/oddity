#include "include.h"

static struct view view;
static char hour[8];
static char minute[8];
static char second[8];

static void time_ondata(unsigned int id, void *data, unsigned int count)
{

    if (count >= 8)
    {

        char *s = data;

        memcpy(hour, s, 2);
        memcpy(minute, s + 3, 2);
        memcpy(second, s + 6, 2);

    }

}

static void onbutton(unsigned int button)
{

    switch (button)
    {

    case BUTTON_A:
        if (view_isactive(&view, "area_save"))
        {

            /* Call helper here */

        }

        break;

    case BUTTON_LEFT:
        if (view_isactive(&view, "area_hour"))
            main_goprev(&view, "select_hour");

        if (view_isactive(&view, "area_minute"))
            main_goprev(&view, "select_minute");

        if (view_isactive(&view, "area_second"))
            main_goprev(&view, "select_second");

        break;

    case BUTTON_RIGHT:
        if (view_isactive(&view, "area_hour"))
            main_gonext(&view, "select_hour");

        if (view_isactive(&view, "area_minute"))
            main_gonext(&view, "select_minute");

        if (view_isactive(&view, "area_second"))
            main_gonext(&view, "select_second");

        break;

    }

}

static void onload(unsigned int type)
{

    view_setattr(&view, "area_hour", "selectable", "true");
    view_setattr(&view, "area_minute", "selectable", "true");
    view_setattr(&view, "area_second", "selectable", "true");
    view_setattr(&view, "area_save", "selectable", "true");
    helper_time_get(1, time_ondata, 0, 0);

}

void view_settings_time_setup(void)
{

    view_init(&view, "settings_time", onload, 0, 0, onbutton);
    pool_create_area(&view, "area_hour", WIDGET_IN_DEFAULT, 2, 2, 8, 1);
    pool_create_area(&view, "area_hour_text", WIDGET_IN_DEFAULT, 2, 2, 4, 1);
    pool_create_area(&view, "area_hour_select", WIDGET_IN_DEFAULT, 6, 2, 4, 1);
    pool_create_area(&view, "area_minute", WIDGET_IN_DEFAULT, 2, 3, 8, 1);
    pool_create_area(&view, "area_minute_text", WIDGET_IN_DEFAULT, 2, 3, 4, 1);
    pool_create_area(&view, "area_minute_select", WIDGET_IN_DEFAULT, 6, 3, 4, 1);
    pool_create_area(&view, "area_second", WIDGET_IN_DEFAULT, 2, 4, 8, 1);
    pool_create_area(&view, "area_second_text", WIDGET_IN_DEFAULT, 2, 4, 4, 1);
    pool_create_area(&view, "area_second_select", WIDGET_IN_DEFAULT, 6, 4, 4, 1);
    pool_create_area(&view, "area_save", WIDGET_IN_DEFAULT, 2, 6, 8, 1);
    pool_create_text(&view, WIDGET_ID_DEFAULT, "area_hour_text", TEXT_TYPE_NORMAL, TEXT_ALIGN_LEFT, "Hour");
    pool_create_text(&view, WIDGET_ID_DEFAULT, "area_minute_text", TEXT_TYPE_NORMAL, TEXT_ALIGN_LEFT, "Minute");
    pool_create_text(&view, WIDGET_ID_DEFAULT, "area_second_text", TEXT_TYPE_NORMAL, TEXT_ALIGN_LEFT, "Second");
    pool_create_text(&view, WIDGET_ID_DEFAULT, "area_save", TEXT_TYPE_SELECT, TEXT_ALIGN_CENTER, "Save");
    pool_create_select(&view, "select_hour", "area_hour_select", hour);
    pool_create_select(&view, "select_minute", "area_minute_select", minute);
    pool_create_select(&view, "select_second", "area_second_select", second);
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_hour", "00", "00");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_hour", "01", "01");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_hour", "02", "02");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_hour", "03", "03");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_hour", "04", "04");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_hour", "05", "05");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_hour", "06", "06");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_hour", "07", "07");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_hour", "08", "08");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_hour", "09", "09");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_hour", "10", "10");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_hour", "11", "11");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_hour", "12", "12");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_hour", "13", "13");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_hour", "14", "14");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_hour", "15", "15");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_hour", "16", "16");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_hour", "17", "17");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_hour", "18", "18");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_hour", "19", "19");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_hour", "20", "20");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_hour", "21", "21");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_hour", "22", "22");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_hour", "23", "23");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_minute", "00", "00");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_minute", "01", "01");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_minute", "02", "02");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_minute", "03", "03");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_minute", "04", "04");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_minute", "05", "05");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_minute", "06", "06");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_minute", "07", "07");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_minute", "08", "08");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_minute", "09", "09");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_minute", "10", "10");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_minute", "11", "11");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_minute", "12", "12");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_minute", "13", "13");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_minute", "14", "14");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_minute", "15", "15");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_minute", "16", "16");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_minute", "17", "17");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_minute", "18", "18");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_minute", "19", "19");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_minute", "20", "20");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_minute", "21", "21");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_minute", "22", "22");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_minute", "23", "23");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_minute", "24", "24");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_minute", "25", "25");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_minute", "26", "26");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_minute", "27", "27");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_minute", "28", "28");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_minute", "29", "29");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_minute", "30", "30");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_minute", "31", "31");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_minute", "32", "32");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_minute", "33", "33");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_minute", "34", "34");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_minute", "35", "35");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_minute", "36", "36");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_minute", "37", "37");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_minute", "38", "38");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_minute", "39", "39");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_minute", "40", "40");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_minute", "41", "41");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_minute", "42", "42");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_minute", "43", "43");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_minute", "44", "44");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_minute", "45", "45");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_minute", "46", "46");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_minute", "47", "47");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_minute", "48", "48");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_minute", "49", "49");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_minute", "50", "50");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_minute", "51", "51");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_minute", "52", "52");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_minute", "53", "53");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_minute", "54", "54");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_minute", "55", "55");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_minute", "56", "56");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_minute", "57", "57");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_minute", "58", "58");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_minute", "59", "59");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_second", "00", "00");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_second", "01", "01");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_second", "02", "02");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_second", "03", "03");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_second", "04", "04");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_second", "05", "05");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_second", "06", "06");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_second", "07", "07");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_second", "08", "08");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_second", "09", "09");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_second", "10", "10");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_second", "11", "11");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_second", "12", "12");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_second", "13", "13");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_second", "14", "14");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_second", "15", "15");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_second", "16", "16");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_second", "17", "17");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_second", "18", "18");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_second", "19", "19");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_second", "20", "20");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_second", "21", "21");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_second", "22", "22");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_second", "23", "23");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_second", "24", "24");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_second", "25", "25");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_second", "26", "26");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_second", "27", "27");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_second", "28", "28");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_second", "29", "29");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_second", "30", "30");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_second", "31", "31");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_second", "32", "32");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_second", "33", "33");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_second", "34", "34");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_second", "35", "35");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_second", "36", "36");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_second", "37", "37");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_second", "38", "38");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_second", "39", "39");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_second", "40", "40");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_second", "41", "41");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_second", "42", "42");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_second", "43", "43");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_second", "44", "44");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_second", "45", "45");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_second", "46", "46");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_second", "47", "47");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_second", "48", "48");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_second", "49", "49");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_second", "50", "50");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_second", "51", "51");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_second", "52", "52");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_second", "53", "53");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_second", "54", "54");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_second", "55", "55");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_second", "56", "56");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_second", "57", "57");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_second", "58", "58");
    pool_create_option(&view, WIDGET_ID_DEFAULT, "select_second", "59", "59");
    main_registerview(&view);

}

