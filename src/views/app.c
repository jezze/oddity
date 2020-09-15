#include "include.h"

static struct view view;
static struct db_app app;

static void button(unsigned int key)
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

    view_unselect(&view, key, view.name);

}

static void load(void)
{

    view_setattr(&view, "area_run", "selectable", "true");
    view_setattr(&view, "area_uninstall", "selectable", "true");
    view_reset(&view);

}

static void config(char *key, void *value)
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

    view_init(&view, "app", load, 0, config, button);
    widget_area_init(view_createwidget(&view), "area_title", WIDGET_IN_DEFAULT, 0, 0, 12, 1);
    widget_area_init(view_createwidget(&view), "area_description", WIDGET_IN_DEFAULT, 0, 1, 12, 4);
    widget_area_init(view_createwidget(&view), "area_run", WIDGET_IN_DEFAULT, 0, 7, 4, 1);
    widget_area_init(view_createwidget(&view), "area_uninstall", WIDGET_IN_DEFAULT, 4, 7, 4, 1);
    widget_text_init(view_createwidget(&view), "text_title", "area_title", TEXT_TYPE_TITLE, TEXT_ALIGN_CENTER, 0);
    widget_text_init(view_createwidget(&view), "text_description", "area_description", TEXT_TYPE_NORMAL, TEXT_ALIGN_LEFT, 0);
    widget_text_init(view_createwidget(&view), WIDGET_ID_DEFAULT, "area_run", TEXT_TYPE_SELECT, TEXT_ALIGN_CENTER, "Start");
    widget_text_init(view_createwidget(&view), WIDGET_ID_DEFAULT, "area_uninstall", TEXT_TYPE_DISABLE, TEXT_ALIGN_CENTER, "Uninstall");
    main_registerview(&view);

}

