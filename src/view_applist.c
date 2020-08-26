#include <stdlib.h>
#include <string.h>
#include "define.h"
#include "box.h"
#include "list.h"
#include "widget.h"
#include "selection.h"
#include "view.h"
#include "db.h"
#include "main.h"

static struct view view;
static struct widget areas[9];
static struct widget texts[17];
static struct db_applist applist;
static unsigned int page;

static void button(unsigned int key)
{

    unsigned int offset = page * 8;
    unsigned int i;

    selection_move(&view.selection, key);

    for (i = offset; i < offset + 8; i++)
    {

        if (i < applist.count)
        {

            struct db_app *app = &applist.items[i];
            unsigned int k = i % 8;

            if (selection_isactive(&view.selection, areas[k].id))
            {

                main_configview("app", "id", app->id);
                selection_select(&view.selection, key, areas[k].id, view.name, "app");

            }

        }

    }

    selection_unselect(&view.selection, key, view.name);

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

    selection_reset(&view.selection);

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

    widget_area_init(&areas[0], "area0", WIDGET_IN_DEFAULT, 0, 0, 8, 1);
    widget_area_init(&areas[1], "area1", WIDGET_IN_DEFAULT, 0, 1, 8, 1);
    widget_area_init(&areas[2], "area2", WIDGET_IN_DEFAULT, 0, 2, 8, 1);
    widget_area_init(&areas[3], "area3", WIDGET_IN_DEFAULT, 0, 3, 8, 1);
    widget_area_init(&areas[4], "area4", WIDGET_IN_DEFAULT, 0, 4, 8, 1);
    widget_area_init(&areas[5], "area5", WIDGET_IN_DEFAULT, 0, 5, 8, 1);
    widget_area_init(&areas[6], "area6", WIDGET_IN_DEFAULT, 0, 6, 8, 1);
    widget_area_init(&areas[7], "area7", WIDGET_IN_DEFAULT, 0, 7, 8, 1);
    widget_area_init(&areas[8], "area8", WIDGET_IN_DEFAULT, 0, 3, 8, 1);
    widget_text_init(&texts[0], WIDGET_ID_DEFAULT, "area0", TEXT_COLOR_SELECT, TEXT_ALIGN_LEFT, 0);
    widget_text_init(&texts[1], WIDGET_ID_DEFAULT, "area0", TEXT_COLOR_NORMAL, TEXT_ALIGN_RIGHT, 0);
    widget_text_init(&texts[2], WIDGET_ID_DEFAULT, "area1", TEXT_COLOR_SELECT, TEXT_ALIGN_LEFT, 0);
    widget_text_init(&texts[3], WIDGET_ID_DEFAULT, "area1", TEXT_COLOR_NORMAL, TEXT_ALIGN_RIGHT, 0);
    widget_text_init(&texts[4], WIDGET_ID_DEFAULT, "area2", TEXT_COLOR_SELECT, TEXT_ALIGN_LEFT, 0);
    widget_text_init(&texts[5], WIDGET_ID_DEFAULT, "area2", TEXT_COLOR_NORMAL, TEXT_ALIGN_RIGHT, 0);
    widget_text_init(&texts[6], WIDGET_ID_DEFAULT, "area3", TEXT_COLOR_SELECT, TEXT_ALIGN_LEFT, 0);
    widget_text_init(&texts[7], WIDGET_ID_DEFAULT, "area3", TEXT_COLOR_NORMAL, TEXT_ALIGN_RIGHT, 0);
    widget_text_init(&texts[8], WIDGET_ID_DEFAULT, "area4", TEXT_COLOR_SELECT, TEXT_ALIGN_LEFT, 0);
    widget_text_init(&texts[9], WIDGET_ID_DEFAULT, "area4", TEXT_COLOR_NORMAL, TEXT_ALIGN_RIGHT, 0);
    widget_text_init(&texts[10], WIDGET_ID_DEFAULT, "area5", TEXT_COLOR_SELECT, TEXT_ALIGN_LEFT, 0);
    widget_text_init(&texts[11], WIDGET_ID_DEFAULT, "area5", TEXT_COLOR_NORMAL, TEXT_ALIGN_RIGHT, 0);
    widget_text_init(&texts[12], WIDGET_ID_DEFAULT, "area6", TEXT_COLOR_SELECT, TEXT_ALIGN_LEFT, 0);
    widget_text_init(&texts[13], WIDGET_ID_DEFAULT, "area6", TEXT_COLOR_NORMAL, TEXT_ALIGN_RIGHT, 0);
    widget_text_init(&texts[14], WIDGET_ID_DEFAULT, "area7", TEXT_COLOR_SELECT, TEXT_ALIGN_LEFT, 0);
    widget_text_init(&texts[15], WIDGET_ID_DEFAULT, "area7", TEXT_COLOR_NORMAL, TEXT_ALIGN_RIGHT, 0);
    widget_text_init(&texts[16], WIDGET_ID_DEFAULT, "area8", TEXT_COLOR_NORMAL, TEXT_ALIGN_CENTER, "No items found.");
    /*
    selection_add(&view.selection, &areas[0]);
    selection_add(&view.selection, &areas[1]);
    selection_add(&view.selection, &areas[2]);
    selection_add(&view.selection, &areas[3]);
    selection_add(&view.selection, &areas[4]);
    selection_add(&view.selection, &areas[5]);
    selection_add(&view.selection, &areas[6]);
    selection_add(&view.selection, &areas[7]);
    */
    view_init(&view, "applist", load, 0, config, button);
    /*
    view_register(&view, &areas[0]);
    view_register(&view, &areas[1]);
    view_register(&view, &areas[2]);
    view_register(&view, &areas[3]);
    view_register(&view, &areas[4]);
    view_register(&view, &areas[5]);
    view_register(&view, &areas[6]);
    view_register(&view, &areas[7]);
    */
    view_register(&view, &areas[8]);
    /*
    view_register(&view, &texts[0]);
    view_register(&view, &texts[1]);
    view_register(&view, &texts[2]);
    view_register(&view, &texts[3]);
    view_register(&view, &texts[4]);
    view_register(&view, &texts[5]);
    view_register(&view, &texts[6]);
    view_register(&view, &texts[7]);
    view_register(&view, &texts[8]);
    view_register(&view, &texts[9]);
    view_register(&view, &texts[10]);
    view_register(&view, &texts[11]);
    view_register(&view, &texts[12]);
    view_register(&view, &texts[13]);
    view_register(&view, &texts[14]);
    view_register(&view, &texts[15]);
    */
    view_register(&view, &texts[16]);
    main_register(&view);

}

