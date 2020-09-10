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

    view_reset(&view);

}

static void config(char *key, void *value)
{

    if (!strcmp(key, "id"))
    {

        db_freeapp(&app);
        db_loadapp(&app, value);

        view_findwidget(&view, "area_title")->payload.text.data = app.name;
        view_findwidget(&view, "area_description")->payload.text.data = app.shortdescription;

    }

}

void view_app_setup(void)
{

    widget_area_init(view_createwidget(&view), "area_title", WIDGET_IN_DEFAULT, 0, 0, 12, 1);
    widget_text_init(view_createwidget(&view), WIDGET_ID_DEFAULT, "area_title", TEXT_COLOR_TITLE, TEXT_ALIGN_CENTER, 0);
    widget_area_init(view_createwidget(&view), "area_description", WIDGET_IN_DEFAULT, 0, 1, 12, 4);
    widget_text_init(view_createwidget(&view), WIDGET_ID_DEFAULT, "area_description", TEXT_COLOR_NORMAL, TEXT_ALIGN_LEFT, 0);
    widget_area_init(view_createwidget(&view), "area_run", WIDGET_IN_DEFAULT, 0, 7, 4, 1);
    widget_text_init(view_createwidget(&view), WIDGET_ID_DEFAULT, "area_run", TEXT_COLOR_SELECT, TEXT_ALIGN_CENTER, "Start");
    widget_area_init(view_createwidget(&view), "area_uninstall", WIDGET_IN_DEFAULT, 4, 7, 4, 1);
    widget_text_init(view_createwidget(&view), WIDGET_ID_DEFAULT, "area_uninstall", TEXT_COLOR_DISABLE, TEXT_ALIGN_CENTER, "Uninstall");
    view_addselection(&view, "area_run");
    view_addselection(&view, "area_uninstall");
    view_init(&view, "app", load, 0, config, button);
    main_registerview(&view);

}

