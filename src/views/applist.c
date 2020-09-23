#include "include.h"

static struct view view;
static struct db_applist applist;
static struct db_app *apps[8];

static void onbutton(unsigned int key)
{

    view_moveselection(&view, key);

    if (view_isactive(&view, "area0"))
    {

        main_configview("app", "id", apps[0]->id);
        view_select(&view, key, "area0", "app");

    }

    if (view_isactive(&view, "area1"))
    {

        main_configview("app", "id", apps[1]->id);
        view_select(&view, key, "area1", "app");

    }

    if (view_isactive(&view, "area2"))
    {

        main_configview("app", "id", apps[2]->id);
        view_select(&view, key, "area2", "app");

    }

    if (view_isactive(&view, "area3"))
    {

        main_configview("app", "id", apps[3]->id);
        view_select(&view, key, "area3", "app");

    }

    if (view_isactive(&view, "area4"))
    {

        main_configview("app", "id", apps[4]->id);
        view_select(&view, key, "area4", "app");

    }

    if (view_isactive(&view, "area5"))
    {

        main_configview("app", "id", apps[5]->id);
        view_select(&view, key, "area5", "app");

    }

    if (view_isactive(&view, "area6"))
    {

        main_configview("app", "id", apps[6]->id);
        view_select(&view, key, "area6", "app");

    }

    if (view_isactive(&view, "area7"))
    {

        main_configview("app", "id", apps[7]->id);
        view_select(&view, key, "area7", "app");

    }

    view_unselect(&view, key);

}

static void onload(unsigned int type)
{

    unsigned int offset = 0;
    unsigned int i;

    for (i = 0; i < 8; i++)
    {

        unsigned int k = offset + i;

        if (k < applist.count)
            apps[i] = &applist.items[k];
        else
            apps[i] = 0;

    }

    view_setattr(&view, "text_name0", "data", (apps[0]) ? apps[0]->name : 0);
    view_setattr(&view, "text_name1", "data", (apps[1]) ? apps[1]->name : 0);
    view_setattr(&view, "text_name2", "data", (apps[2]) ? apps[2]->name : 0);
    view_setattr(&view, "text_name3", "data", (apps[3]) ? apps[3]->name : 0);
    view_setattr(&view, "text_name4", "data", (apps[4]) ? apps[4]->name : 0);
    view_setattr(&view, "text_name5", "data", (apps[5]) ? apps[5]->name : 0);
    view_setattr(&view, "text_name6", "data", (apps[6]) ? apps[6]->name : 0);
    view_setattr(&view, "text_name7", "data", (apps[7]) ? apps[7]->name : 0);
    view_setattr(&view, "area0", "selectable", apps[0] ? "true" : "false");
    view_setattr(&view, "area1", "selectable", apps[1] ? "true" : "false");
    view_setattr(&view, "area2", "selectable", apps[2] ? "true" : "false");
    view_setattr(&view, "area3", "selectable", apps[3] ? "true" : "false");
    view_setattr(&view, "area4", "selectable", apps[4] ? "true" : "false");
    view_setattr(&view, "area5", "selectable", apps[5] ? "true" : "false");
    view_setattr(&view, "area6", "selectable", apps[6] ? "true" : "false");
    view_setattr(&view, "area7", "selectable", apps[7] ? "true" : "false");
    view_setattr(&view, "text_noitems", "hidden", applist.count ? "true" : "false");

}

static void onconfig(char *key, void *value)
{

    if (!strcmp(key, "list"))
    {

        char *category = value;

        if (applist.count)
            db_freeapps(&applist);

        if (!strcmp(category, "all"))
            db_loadapps(&applist);

        if (!strcmp(category, "installed"))
            db_loadapps(&applist);

        if (!strcmp(category, "new"))
            db_loadapps(&applist);

        if (!strcmp(category, "updated"))
            db_loadapps(&applist);

    }

}

void view_applist_setup(void)
{

    view_init(&view, "applist", onload, 0, onconfig, onbutton);
    pool_create_area(&view, "area0", WIDGET_IN_DEFAULT, 0, 0, 12, 1);
    pool_create_area(&view, "area1", WIDGET_IN_DEFAULT, 0, 1, 12, 1);
    pool_create_area(&view, "area2", WIDGET_IN_DEFAULT, 0, 2, 12, 1);
    pool_create_area(&view, "area3", WIDGET_IN_DEFAULT, 0, 3, 12, 1);
    pool_create_area(&view, "area4", WIDGET_IN_DEFAULT, 0, 4, 12, 1);
    pool_create_area(&view, "area5", WIDGET_IN_DEFAULT, 0, 5, 12, 1);
    pool_create_area(&view, "area6", WIDGET_IN_DEFAULT, 0, 6, 12, 1);
    pool_create_area(&view, "area7", WIDGET_IN_DEFAULT, 0, 7, 12, 1);
    pool_create_area(&view, "area_noitems", WIDGET_IN_DEFAULT, 0, 3, 12, 1);
    pool_create_text(&view, "text_name0", "area0", TEXT_TYPE_SELECT, TEXT_ALIGN_LEFT, 0);
    pool_create_text(&view, WIDGET_ID_DEFAULT, "area0", TEXT_TYPE_NORMAL, TEXT_ALIGN_RIGHT, 0);
    pool_create_text(&view, "text_name1", "area1", TEXT_TYPE_SELECT, TEXT_ALIGN_LEFT, 0);
    pool_create_text(&view, WIDGET_ID_DEFAULT, "area1", TEXT_TYPE_NORMAL, TEXT_ALIGN_RIGHT, 0);
    pool_create_text(&view, "text_name2", "area2", TEXT_TYPE_SELECT, TEXT_ALIGN_LEFT, 0);
    pool_create_text(&view, WIDGET_ID_DEFAULT, "area2", TEXT_TYPE_NORMAL, TEXT_ALIGN_RIGHT, 0);
    pool_create_text(&view, "text_name3", "area3", TEXT_TYPE_SELECT, TEXT_ALIGN_LEFT, 0);
    pool_create_text(&view, WIDGET_ID_DEFAULT, "area3", TEXT_TYPE_NORMAL, TEXT_ALIGN_RIGHT, 0);
    pool_create_text(&view, "text_name4", "area4", TEXT_TYPE_SELECT, TEXT_ALIGN_LEFT, 0);
    pool_create_text(&view, WIDGET_ID_DEFAULT, "area4", TEXT_TYPE_NORMAL, TEXT_ALIGN_RIGHT, 0);
    pool_create_text(&view, "text_name5", "area5", TEXT_TYPE_SELECT, TEXT_ALIGN_LEFT, 0);
    pool_create_text(&view, WIDGET_ID_DEFAULT, "area5", TEXT_TYPE_NORMAL, TEXT_ALIGN_RIGHT, 0);
    pool_create_text(&view, "text_name6", "area6", TEXT_TYPE_SELECT, TEXT_ALIGN_LEFT, 0);
    pool_create_text(&view, WIDGET_ID_DEFAULT, "area6", TEXT_TYPE_NORMAL, TEXT_ALIGN_RIGHT, 0);
    pool_create_text(&view, "text_name7", "area7", TEXT_TYPE_SELECT, TEXT_ALIGN_LEFT, 0);
    pool_create_text(&view, WIDGET_ID_DEFAULT, "area7", TEXT_TYPE_NORMAL, TEXT_ALIGN_RIGHT, 0);
    pool_create_text(&view, "text_noitems", "area_noitems", TEXT_TYPE_NORMAL, TEXT_ALIGN_CENTER, "No items found.");
    main_registerview(&view);

}

