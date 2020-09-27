#include "include.h"

struct entry
{

    unsigned int active;
    char *name;
    char *area;
    char *text;

};

static struct view view;
static char text[0x2000];
static unsigned int offset;
static struct entry continents[10];
static struct entry countries[500];
static unsigned int ncontinents;
static unsigned int ncountries;

static void ondata(unsigned int id, void *data, unsigned int count)
{

    memcpy(text + offset, data, count);

    offset += count;

}

static void continents_oncomplete(unsigned int id)
{

    unsigned int i;
    char *start = text;

    for (i = 0; i < offset; i++)
    {

        if (text[i] == '\n')
        {

            continents[ncontinents].name = start;
            continents[ncontinents].active = 1;

            start = text + i + 1;

            if (++ncontinents >= 10)
                break;

        }

    }

    if (ncontinents)
    {

        unsigned int max = (ncontinents > 10) ? 10 : ncontinents;

        for (i = 0; i < max; i++)
        {

            struct entry *c = &continents[i];

            if (c->active)
            {

                view_setattr(&view, c->area, "selectable", "true");
                view_setattr(&view, c->text, "data", c->name);

            }

        }

    }

    else
    {

        view_setattr(&view, "text_noitems", "hidden", "false");

    }

}

static void countries_oncomplete(unsigned int id)
{

    unsigned int i;
    char *start = text;

    for (i = 0; i < offset; i++)
    {

        if (text[i] == '\n')
        {

            countries[ncountries].name = start;
            countries[ncountries].active = 1;

            start = text + i + 1;

            if (++ncountries >= 500)
                break;

        }

    }

    if (ncountries)
    {

        unsigned int max = (ncountries > 16) ? 16 : ncountries;

        for (i = 0; i < max; i++)
        {

            struct entry *c = &countries[i];

            if (c->active)
            {

                view_setattr(&view, c->area, "selectable", "true");
                view_setattr(&view, c->text, "data", c->name);

            }

        }

    }

    else
    {

        view_setattr(&view, "text_noitems", "hidden", "false");

    }

}

static void onload(unsigned int type)
{

    offset = 0;
    ncontinents = 0;
    ncountries = 0;

    continents[0].area = "area_continent0";
    continents[1].area = "area_continent1";
    continents[2].area = "area_continent2";
    continents[3].area = "area_continent3";
    continents[4].area = "area_continent4";
    continents[5].area = "area_continent5";
    continents[6].area = "area_continent6";
    continents[7].area = "area_continent7";
    continents[8].area = "area_continent8";
    continents[9].area = "area_continent9";    
    continents[0].text = "text_continent0";
    continents[1].text = "text_continent1";
    continents[2].text = "text_continent2";
    continents[3].text = "text_continent3";
    continents[4].text = "text_continent4";
    continents[5].text = "text_continent5";
    continents[6].text = "text_continent6";
    continents[7].text = "text_continent7";
    continents[8].text = "text_continent8";
    continents[9].text = "text_continent9";
    countries[0].area = "area_country0";
    countries[1].area = "area_country1";
    countries[2].area = "area_country2";
    countries[3].area = "area_country3";
    countries[4].area = "area_country4";
    countries[5].area = "area_country5";
    countries[6].area = "area_country6";
    countries[7].area = "area_country7";
    countries[8].area = "area_country8";
    countries[9].area = "area_country9";
    countries[10].area = "area_country10";
    countries[11].area = "area_country11";
    countries[12].area = "area_country12";
    countries[13].area = "area_country13";
    countries[14].area = "area_country14";
    countries[15].area = "area_country15";
    countries[0].text = "text_country0";
    countries[1].text = "text_country1";
    countries[2].text = "text_country2";
    countries[3].text = "text_country3";
    countries[4].text = "text_country4";
    countries[5].text = "text_country5";
    countries[6].text = "text_country6";
    countries[7].text = "text_country7";
    countries[8].text = "text_country8";
    countries[9].text = "text_country9";
    countries[10].text = "text_country10";
    countries[11].text = "text_country11";
    countries[12].text = "text_country12";
    countries[13].text = "text_country13";
    countries[14].text = "text_country14";
    countries[15].text = "text_country15";

    view_setattr(&view, "text_noitems", "hidden", "true");

    if (type == VIEW_LOADTYPE_INIT)
        helper_timezone_getcontinents(1, ondata, continents_oncomplete, 0);
    else
        helper_timezone_getcountries(1, "Europe", ondata, countries_oncomplete, 0);

}

static void onbutton(unsigned int button)
{

    if (button == BUTTON_A)
    {

        onload(32);

    }

}

void view_settings_timezone_setup(void)
{

    view_init(&view, "settings_timezone", onload, 0, 0, onbutton);
    pool_create_area(&view, "area_continent0", WIDGET_IN_DEFAULT, 0, 2, 6, 1);
    pool_create_area(&view, "area_continent1", WIDGET_IN_DEFAULT, 0, 3, 6, 1);
    pool_create_area(&view, "area_continent2", WIDGET_IN_DEFAULT, 0, 4, 6, 1);
    pool_create_area(&view, "area_continent3", WIDGET_IN_DEFAULT, 0, 5, 6, 1);
    pool_create_area(&view, "area_continent4", WIDGET_IN_DEFAULT, 0, 6, 6, 1);
    pool_create_area(&view, "area_continent5", WIDGET_IN_DEFAULT, 6, 2, 6, 1);
    pool_create_area(&view, "area_continent6", WIDGET_IN_DEFAULT, 6, 3, 6, 1);
    pool_create_area(&view, "area_continent7", WIDGET_IN_DEFAULT, 6, 4, 6, 1);
    pool_create_area(&view, "area_continent8", WIDGET_IN_DEFAULT, 6, 5, 6, 1);
    pool_create_area(&view, "area_continent9", WIDGET_IN_DEFAULT, 6, 6, 6, 1);
    pool_create_area(&view, "area_country0", WIDGET_IN_DEFAULT, 0, 0, 6, 1);
    pool_create_area(&view, "area_country1", WIDGET_IN_DEFAULT, 0, 1, 6, 1);
    pool_create_area(&view, "area_country2", WIDGET_IN_DEFAULT, 0, 2, 6, 1);
    pool_create_area(&view, "area_country3", WIDGET_IN_DEFAULT, 0, 3, 6, 1);
    pool_create_area(&view, "area_country4", WIDGET_IN_DEFAULT, 0, 4, 6, 1);
    pool_create_area(&view, "area_country5", WIDGET_IN_DEFAULT, 0, 5, 6, 1);
    pool_create_area(&view, "area_country6", WIDGET_IN_DEFAULT, 0, 6, 6, 1);
    pool_create_area(&view, "area_country7", WIDGET_IN_DEFAULT, 0, 7, 6, 1);
    pool_create_area(&view, "area_country8", WIDGET_IN_DEFAULT, 6, 0, 6, 1);
    pool_create_area(&view, "area_country9", WIDGET_IN_DEFAULT, 6, 1, 6, 1);
    pool_create_area(&view, "area_country10", WIDGET_IN_DEFAULT, 6, 2, 6, 1);
    pool_create_area(&view, "area_country11", WIDGET_IN_DEFAULT, 6, 3, 6, 1);
    pool_create_area(&view, "area_country12", WIDGET_IN_DEFAULT, 6, 4, 6, 1);
    pool_create_area(&view, "area_country13", WIDGET_IN_DEFAULT, 6, 5, 6, 1);
    pool_create_area(&view, "area_country14", WIDGET_IN_DEFAULT, 6, 6, 6, 1);
    pool_create_area(&view, "area_country15", WIDGET_IN_DEFAULT, 6, 7, 6, 1);
    pool_create_area(&view, "area_noitems", WIDGET_IN_DEFAULT, 0, 3, 12, 1);
    pool_create_text(&view, "text_continent0", "area_continent0", TEXT_TYPE_SELECT, TEXT_ALIGN_CENTER, 0);
    pool_create_text(&view, "text_continent1", "area_continent1", TEXT_TYPE_SELECT, TEXT_ALIGN_CENTER, 0);
    pool_create_text(&view, "text_continent2", "area_continent2", TEXT_TYPE_SELECT, TEXT_ALIGN_CENTER, 0);
    pool_create_text(&view, "text_continent3", "area_continent3", TEXT_TYPE_SELECT, TEXT_ALIGN_CENTER, 0);
    pool_create_text(&view, "text_continent4", "area_continent4", TEXT_TYPE_SELECT, TEXT_ALIGN_CENTER, 0);
    pool_create_text(&view, "text_continent5", "area_continent5", TEXT_TYPE_SELECT, TEXT_ALIGN_CENTER, 0);
    pool_create_text(&view, "text_continent6", "area_continent6", TEXT_TYPE_SELECT, TEXT_ALIGN_CENTER, 0);
    pool_create_text(&view, "text_continent7", "area_continent7", TEXT_TYPE_SELECT, TEXT_ALIGN_CENTER, 0);
    pool_create_text(&view, "text_continent8", "area_continent8", TEXT_TYPE_SELECT, TEXT_ALIGN_CENTER, 0);
    pool_create_text(&view, "text_continent9", "area_continent9", TEXT_TYPE_SELECT, TEXT_ALIGN_CENTER, 0);
    pool_create_text(&view, "text_country0", "area_country0", TEXT_TYPE_SELECT, TEXT_ALIGN_CENTER, 0);
    pool_create_text(&view, "text_country1", "area_country1", TEXT_TYPE_SELECT, TEXT_ALIGN_CENTER, 0);
    pool_create_text(&view, "text_country2", "area_country2", TEXT_TYPE_SELECT, TEXT_ALIGN_CENTER, 0);
    pool_create_text(&view, "text_country3", "area_country3", TEXT_TYPE_SELECT, TEXT_ALIGN_CENTER, 0);
    pool_create_text(&view, "text_country4", "area_country4", TEXT_TYPE_SELECT, TEXT_ALIGN_CENTER, 0);
    pool_create_text(&view, "text_country5", "area_country5", TEXT_TYPE_SELECT, TEXT_ALIGN_CENTER, 0);
    pool_create_text(&view, "text_country6", "area_country6", TEXT_TYPE_SELECT, TEXT_ALIGN_CENTER, 0);
    pool_create_text(&view, "text_country7", "area_country7", TEXT_TYPE_SELECT, TEXT_ALIGN_CENTER, 0);
    pool_create_text(&view, "text_country8", "area_country8", TEXT_TYPE_SELECT, TEXT_ALIGN_CENTER, 0);
    pool_create_text(&view, "text_country9", "area_country9", TEXT_TYPE_SELECT, TEXT_ALIGN_CENTER, 0);
    pool_create_text(&view, "text_country10", "area_country10", TEXT_TYPE_SELECT, TEXT_ALIGN_CENTER, 0);
    pool_create_text(&view, "text_country11", "area_country11", TEXT_TYPE_SELECT, TEXT_ALIGN_CENTER, 0);
    pool_create_text(&view, "text_country12", "area_country12", TEXT_TYPE_SELECT, TEXT_ALIGN_CENTER, 0);
    pool_create_text(&view, "text_country13", "area_country13", TEXT_TYPE_SELECT, TEXT_ALIGN_CENTER, 0);
    pool_create_text(&view, "text_country14", "area_country14", TEXT_TYPE_SELECT, TEXT_ALIGN_CENTER, 0);
    pool_create_text(&view, "text_country15", "area_country15", TEXT_TYPE_SELECT, TEXT_ALIGN_CENTER, 0);
    pool_create_text(&view, "text_noitems", "area_noitems", TEXT_TYPE_NORMAL, TEXT_ALIGN_CENTER, "No items found.");
    main_registerview(&view);

}

