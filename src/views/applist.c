#include "include.h"

static struct view view;
static struct db_applist applist;
static unsigned int page;
static char *names[8];

static void button(unsigned int key)
{

    unsigned int offset = page * 8;
    unsigned int i;

    view_moveselection(&view, key);

    for (i = offset; i < offset + 8; i++)
    {

        if (i < applist.count)
        {

/*
            struct db_app *app = &applist.items[i];
            unsigned int k = i % 8;

            if (view_isactive(&view, areas[k].id))
            {

                main_configview("app", "id", app->id);
                view_select(&view, key, areas[k].id, view.name, "app");

            }
*/

        }

    }

    view_unselect(&view, key, view.name);

    switch (key)
    {

    case KEY_LEFT:
        if (page > 0)
            page--;

        break;

    case KEY_RIGHT:
        if (page < (applist.count / 8))
            page++;

        break;

    }

}

static void load(unsigned int type)
{

    unsigned int i;

    page = 0;

    for (i = 0; i < 8; i++)
    {

        names[i] = "";

    }

    for (i = 0; i < applist.count; i++)
    {

        names[i] = applist.items[i].name;

    }

}

static void config(char *key, void *value)
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

    view_init(&view, "applist", load, 0, config, button);
    pool_area_create(&view, "area0", WIDGET_IN_DEFAULT, 0, 0, 12, 1);
    pool_area_create(&view, "area1", WIDGET_IN_DEFAULT, 0, 1, 12, 1);
    pool_area_create(&view, "area2", WIDGET_IN_DEFAULT, 0, 2, 12, 1);
    pool_area_create(&view, "area3", WIDGET_IN_DEFAULT, 0, 3, 12, 1);
    pool_area_create(&view, "area4", WIDGET_IN_DEFAULT, 0, 4, 12, 1);
    pool_area_create(&view, "area5", WIDGET_IN_DEFAULT, 0, 5, 12, 1);
    pool_area_create(&view, "area6", WIDGET_IN_DEFAULT, 0, 6, 12, 1);
    pool_area_create(&view, "area7", WIDGET_IN_DEFAULT, 0, 7, 12, 1);
    pool_area_create(&view, "area_noitems", WIDGET_IN_DEFAULT, 0, 3, 12, 1);
    pool_text_create(&view, WIDGET_ID_DEFAULT, "area0", TEXT_TYPE_SELECT, TEXT_ALIGN_LEFT, names[0]);
    pool_text_create(&view, WIDGET_ID_DEFAULT, "area0", TEXT_TYPE_NORMAL, TEXT_ALIGN_RIGHT, 0);
    pool_text_create(&view, WIDGET_ID_DEFAULT, "area1", TEXT_TYPE_SELECT, TEXT_ALIGN_LEFT, names[1]);
    pool_text_create(&view, WIDGET_ID_DEFAULT, "area1", TEXT_TYPE_NORMAL, TEXT_ALIGN_RIGHT, 0);
    pool_text_create(&view, WIDGET_ID_DEFAULT, "area2", TEXT_TYPE_SELECT, TEXT_ALIGN_LEFT, names[2]);
    pool_text_create(&view, WIDGET_ID_DEFAULT, "area2", TEXT_TYPE_NORMAL, TEXT_ALIGN_RIGHT, 0);
    pool_text_create(&view, WIDGET_ID_DEFAULT, "area3", TEXT_TYPE_SELECT, TEXT_ALIGN_LEFT, names[3]);
    pool_text_create(&view, WIDGET_ID_DEFAULT, "area3", TEXT_TYPE_NORMAL, TEXT_ALIGN_RIGHT, 0);
    pool_text_create(&view, WIDGET_ID_DEFAULT, "area4", TEXT_TYPE_SELECT, TEXT_ALIGN_LEFT, names[4]);
    pool_text_create(&view, WIDGET_ID_DEFAULT, "area4", TEXT_TYPE_NORMAL, TEXT_ALIGN_RIGHT, 0);
    pool_text_create(&view, WIDGET_ID_DEFAULT, "area5", TEXT_TYPE_SELECT, TEXT_ALIGN_LEFT, names[5]);
    pool_text_create(&view, WIDGET_ID_DEFAULT, "area5", TEXT_TYPE_NORMAL, TEXT_ALIGN_RIGHT, 0);
    pool_text_create(&view, WIDGET_ID_DEFAULT, "area6", TEXT_TYPE_SELECT, TEXT_ALIGN_LEFT, names[6]);
    pool_text_create(&view, WIDGET_ID_DEFAULT, "area6", TEXT_TYPE_NORMAL, TEXT_ALIGN_RIGHT, 0);
    pool_text_create(&view, WIDGET_ID_DEFAULT, "area7", TEXT_TYPE_SELECT, TEXT_ALIGN_LEFT, names[7]);
    pool_text_create(&view, WIDGET_ID_DEFAULT, "area7", TEXT_TYPE_NORMAL, TEXT_ALIGN_RIGHT, 0);
    pool_text_create(&view, WIDGET_ID_DEFAULT, "area_noitems", TEXT_TYPE_NORMAL, TEXT_ALIGN_CENTER, "No items found.");
    main_registerview(&view);

}

