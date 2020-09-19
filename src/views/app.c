#include "include.h"

static struct view view;
static struct db_app app;

static void onbutton(unsigned int key)
{

    view_moveselection(&view, key);

    if (key == KEY_A)
    {

        if (view_isactive(&view, "area_run"))
        {

            struct db_packagelist packagelist;
            unsigned int i;

            db_loadpackagesfromapp(&packagelist, &app);

            for (i = 0; i < packagelist.count; i++)
            {

                main_exec(packagelist.items[i].sha1);

                break;

            }

            db_freepackages(&packagelist);

        }

    }

    view_unselect(&view, key);

}

static void onload(unsigned int type)
{

    view_setattr(&view, "area_run", "selectable", "true");
    view_setattr(&view, "area_uninstall", "selectable", "true");

}

static void onconfig(char *key, void *value)
{

    if (!strcmp(key, "id"))
    {

        db_freeapp(&app);
        db_loadapp(&app, value);
        view_setattr(&view, "text_title", "data", app.name);
        view_setattr(&view, "text_description", "data", app.shortdescription);

    }

}

void view_app_setup(void)
{

    view_init(&view, "app", onload, 0, onconfig, onbutton);
    pool_create_area(&view, "area_title", WIDGET_IN_DEFAULT, 0, 0, 12, 1);
    pool_create_area(&view, "area_description", WIDGET_IN_DEFAULT, 0, 1, 12, 4);
    pool_create_area(&view, "area_run", WIDGET_IN_DEFAULT, 0, 7, 4, 1);
    pool_create_area(&view, "area_uninstall", WIDGET_IN_DEFAULT, 4, 7, 4, 1);
    pool_create_text(&view, "text_title", "area_title", TEXT_TYPE_TITLE, TEXT_ALIGN_CENTER, 0);
    pool_create_text(&view, "text_description", "area_description", TEXT_TYPE_NORMAL, TEXT_ALIGN_LEFT, 0);
    pool_create_text(&view, WIDGET_ID_DEFAULT, "area_run", TEXT_TYPE_SELECT, TEXT_ALIGN_CENTER, "Start");
    pool_create_text(&view, WIDGET_ID_DEFAULT, "area_uninstall", TEXT_TYPE_DISABLE, TEXT_ALIGN_CENTER, "Uninstall");
    main_registerview(&view);

}

