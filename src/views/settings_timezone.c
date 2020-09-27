#include "include.h"

static struct continent
{

    unsigned int active;
    char *name;
    char *area;
    char *text;

} continents[16];

static struct view view;
static char text[1024];
static unsigned int offset;
static unsigned int nentries;

static void continents_ondata(unsigned int id, void *data, unsigned int count)
{

    memcpy(text + offset, data, count);

    offset += count;

}

static void continents_oncomplete(unsigned int id)
{

    unsigned int i;

    continents[0].name = text;

    for (i = 0; i < offset; i++)
    {

        if (text[i] == '\n')
        {

            continents[nentries].active = 1;

            nentries++;

            continents[nentries].name = text + i + 1;

        }

    }

    if (nentries)
    {

        for (i = 0; i < nentries; i++)
        {

            struct continent *c = &continents[i];

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
    nentries = 0;

    continents[0].area = "area0";
    continents[1].area = "area1";
    continents[2].area = "area2";
    continents[3].area = "area3";
    continents[4].area = "area4";
    continents[5].area = "area5";
    continents[6].area = "area6";
    continents[7].area = "area7";
    continents[8].area = "area8";
    continents[9].area = "area9";
    continents[10].area = "area10";
    continents[11].area = "area11";
    continents[12].area = "area12";
    continents[13].area = "area13";
    continents[14].area = "area14";
    continents[15].area = "area15";
    continents[0].text = "text0";
    continents[1].text = "text1";
    continents[2].text = "text2";
    continents[3].text = "text3";
    continents[4].text = "text4";
    continents[5].text = "text5";
    continents[6].text = "text6";
    continents[7].text = "text7";
    continents[8].text = "text8";
    continents[9].text = "text9";
    continents[10].text = "text10";
    continents[11].text = "text11";
    continents[12].text = "text12";
    continents[13].text = "text13";
    continents[14].text = "text14";
    continents[15].text = "text15";

    view_setattr(&view, "text_noitems", "hidden", "true");
    helper_timezone_getcontinents(1, continents_ondata, continents_oncomplete, 0);

}

void view_settings_timezone_setup(void)
{

    view_init(&view, "settings_timezone", onload, 0, 0, 0);
    pool_create_area(&view, "area0", WIDGET_IN_DEFAULT, 0, 0, 6, 1);
    pool_create_area(&view, "area1", WIDGET_IN_DEFAULT, 0, 1, 6, 1);
    pool_create_area(&view, "area2", WIDGET_IN_DEFAULT, 0, 2, 6, 1);
    pool_create_area(&view, "area3", WIDGET_IN_DEFAULT, 0, 3, 6, 1);
    pool_create_area(&view, "area4", WIDGET_IN_DEFAULT, 0, 4, 6, 1);
    pool_create_area(&view, "area5", WIDGET_IN_DEFAULT, 0, 5, 6, 1);
    pool_create_area(&view, "area6", WIDGET_IN_DEFAULT, 0, 6, 6, 1);
    pool_create_area(&view, "area7", WIDGET_IN_DEFAULT, 0, 7, 6, 1);
    pool_create_area(&view, "area8", WIDGET_IN_DEFAULT, 6, 0, 6, 1);
    pool_create_area(&view, "area9", WIDGET_IN_DEFAULT, 6, 1, 6, 1);
    pool_create_area(&view, "area10", WIDGET_IN_DEFAULT, 6, 2, 6, 1);
    pool_create_area(&view, "area11", WIDGET_IN_DEFAULT, 6, 3, 6, 1);
    pool_create_area(&view, "area12", WIDGET_IN_DEFAULT, 6, 4, 6, 1);
    pool_create_area(&view, "area13", WIDGET_IN_DEFAULT, 6, 5, 6, 1);
    pool_create_area(&view, "area14", WIDGET_IN_DEFAULT, 6, 6, 6, 1);
    pool_create_area(&view, "area15", WIDGET_IN_DEFAULT, 6, 7, 6, 1);
    pool_create_area(&view, "area_noitems", WIDGET_IN_DEFAULT, 0, 3, 12, 1);
    pool_create_text(&view, "text0", "area0", TEXT_TYPE_SELECT, TEXT_ALIGN_CENTER, 0);
    pool_create_text(&view, "text1", "area1", TEXT_TYPE_SELECT, TEXT_ALIGN_CENTER, 0);
    pool_create_text(&view, "text2", "area2", TEXT_TYPE_SELECT, TEXT_ALIGN_CENTER, 0);
    pool_create_text(&view, "text3", "area3", TEXT_TYPE_SELECT, TEXT_ALIGN_CENTER, 0);
    pool_create_text(&view, "text4", "area4", TEXT_TYPE_SELECT, TEXT_ALIGN_CENTER, 0);
    pool_create_text(&view, "text5", "area5", TEXT_TYPE_SELECT, TEXT_ALIGN_CENTER, 0);
    pool_create_text(&view, "text6", "area6", TEXT_TYPE_SELECT, TEXT_ALIGN_CENTER, 0);
    pool_create_text(&view, "text7", "area7", TEXT_TYPE_SELECT, TEXT_ALIGN_CENTER, 0);
    pool_create_text(&view, "text8", "area8", TEXT_TYPE_SELECT, TEXT_ALIGN_CENTER, 0);
    pool_create_text(&view, "text9", "area9", TEXT_TYPE_SELECT, TEXT_ALIGN_CENTER, 0);
    pool_create_text(&view, "text10", "area10", TEXT_TYPE_SELECT, TEXT_ALIGN_CENTER, 0);
    pool_create_text(&view, "text11", "area11", TEXT_TYPE_SELECT, TEXT_ALIGN_CENTER, 0);
    pool_create_text(&view, "text12", "area12", TEXT_TYPE_SELECT, TEXT_ALIGN_CENTER, 0);
    pool_create_text(&view, "text13", "area13", TEXT_TYPE_SELECT, TEXT_ALIGN_CENTER, 0);
    pool_create_text(&view, "text14", "area14", TEXT_TYPE_SELECT, TEXT_ALIGN_CENTER, 0);
    pool_create_text(&view, "text15", "area15", TEXT_TYPE_SELECT, TEXT_ALIGN_CENTER, 0);
    pool_create_text(&view, "text_noitems", "area_noitems", TEXT_TYPE_NORMAL, TEXT_ALIGN_CENTER, "No items found.");
    main_registerview(&view);

}

