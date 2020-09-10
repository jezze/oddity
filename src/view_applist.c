#include <stdlib.h>
#include <string.h>
#include "define.h"
#include "box.h"
#include "list.h"
#include "widget.h"
#include "view.h"
#include "db.h"
#include "main.h"

static struct view view;
static struct db_applist applist;
static unsigned int page;

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

static void load(void)
{

    page = 0;

    view_reset(&view);

}

static void config(char *key, void *value)
{

    if (!strcmp(key, "list"))
    {

        char *category = value;

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

/*
    widget_area_init(view_createwidget(&view), "area0", WIDGET_IN_DEFAULT, 0, 0, 8, 1);
    widget_area_init(view_createwidget(&view), "area1", WIDGET_IN_DEFAULT, 0, 1, 8, 1);
    widget_area_init(view_createwidget(&view), "area2", WIDGET_IN_DEFAULT, 0, 2, 8, 1);
    widget_area_init(view_createwidget(&view), "area3", WIDGET_IN_DEFAULT, 0, 3, 8, 1);
    widget_area_init(view_createwidget(&view), "area4", WIDGET_IN_DEFAULT, 0, 4, 8, 1);
    widget_area_init(view_createwidget(&view), "area5", WIDGET_IN_DEFAULT, 0, 5, 8, 1);
    widget_area_init(view_createwidget(&view), "area6", WIDGET_IN_DEFAULT, 0, 6, 8, 1);
    widget_area_init(view_createwidget(&view), "area7", WIDGET_IN_DEFAULT, 0, 7, 8, 1);
    widget_text_init(view_createwidget(&view), WIDGET_ID_DEFAULT, "area0", TEXT_COLOR_SELECT, TEXT_ALIGN_LEFT, 0);
    widget_text_init(view_createwidget(&view), WIDGET_ID_DEFAULT, "area0", TEXT_COLOR_NORMAL, TEXT_ALIGN_RIGHT, 0);
    widget_text_init(view_createwidget(&view), WIDGET_ID_DEFAULT, "area1", TEXT_COLOR_SELECT, TEXT_ALIGN_LEFT, 0);
    widget_text_init(view_createwidget(&view), WIDGET_ID_DEFAULT, "area1", TEXT_COLOR_NORMAL, TEXT_ALIGN_RIGHT, 0);
    widget_text_init(view_createwidget(&view), WIDGET_ID_DEFAULT, "area2", TEXT_COLOR_SELECT, TEXT_ALIGN_LEFT, 0);
    widget_text_init(view_createwidget(&view), WIDGET_ID_DEFAULT, "area2", TEXT_COLOR_NORMAL, TEXT_ALIGN_RIGHT, 0);
    widget_text_init(view_createwidget(&view), WIDGET_ID_DEFAULT, "area3", TEXT_COLOR_SELECT, TEXT_ALIGN_LEFT, 0);
    widget_text_init(view_createwidget(&view), WIDGET_ID_DEFAULT, "area3", TEXT_COLOR_NORMAL, TEXT_ALIGN_RIGHT, 0);
    widget_text_init(view_createwidget(&view), WIDGET_ID_DEFAULT, "area4", TEXT_COLOR_SELECT, TEXT_ALIGN_LEFT, 0);
    widget_text_init(view_createwidget(&view), WIDGET_ID_DEFAULT, "area4", TEXT_COLOR_NORMAL, TEXT_ALIGN_RIGHT, 0);
    widget_text_init(view_createwidget(&view), WIDGET_ID_DEFAULT, "area5", TEXT_COLOR_SELECT, TEXT_ALIGN_LEFT, 0);
    widget_text_init(view_createwidget(&view), WIDGET_ID_DEFAULT, "area5", TEXT_COLOR_NORMAL, TEXT_ALIGN_RIGHT, 0);
    widget_text_init(view_createwidget(&view), WIDGET_ID_DEFAULT, "area6", TEXT_COLOR_SELECT, TEXT_ALIGN_LEFT, 0);
    widget_text_init(view_createwidget(&view), WIDGET_ID_DEFAULT, "area6", TEXT_COLOR_NORMAL, TEXT_ALIGN_RIGHT, 0);
    widget_text_init(view_createwidget(&view), WIDGET_ID_DEFAULT, "area7", TEXT_COLOR_SELECT, TEXT_ALIGN_LEFT, 0);
    widget_text_init(view_createwidget(&view), WIDGET_ID_DEFAULT, "area7", TEXT_COLOR_NORMAL, TEXT_ALIGN_RIGHT, 0);
*/

    widget_area_init(view_createwidget(&view), "area_noitems", WIDGET_IN_DEFAULT, 0, 3, 12, 1);
    widget_text_init(view_createwidget(&view), WIDGET_ID_DEFAULT, "area_noitems", TEXT_COLOR_NORMAL, TEXT_ALIGN_CENTER, "No items found.");
    view_init(&view, "applist", load, 0, config, button);
    main_registerview(&view);

}

