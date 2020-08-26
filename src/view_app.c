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
static struct widget titlearea;
static struct widget titletext;
static struct widget descriptionarea;
static struct widget descriptiontext;
static struct widget runarea;
static struct widget runtext;
static struct widget uninstallarea;
static struct widget uninstalltext;
static struct db_app app;

static void button(unsigned int key)
{

    view_moveselection(&view, key);

    if (key == KEY_A)
    {

        if (view_isactive(&view, "runarea"))
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

        titletext.payload.text.data = app.name;
        descriptiontext.payload.text.data = app.shortdescription;

    }

}

void view_app_setup(void)
{

    widget_area_init(&titlearea, "titlearea", WIDGET_IN_DEFAULT, 0, 0, 8, 1);
    widget_text_init(&titletext, WIDGET_ID_DEFAULT, "titlearea", TEXT_COLOR_TITLE, TEXT_ALIGN_CENTER, 0);
    widget_area_init(&descriptionarea, "descriptionarea", WIDGET_IN_DEFAULT, 0, 1, 8, 4);
    widget_text_init(&descriptiontext, WIDGET_ID_DEFAULT, "descriptionarea", TEXT_COLOR_NORMAL, TEXT_ALIGN_LEFT, 0);
    widget_area_init(&runarea, "runarea", WIDGET_IN_DEFAULT, 0, 7, 4, 1);
    widget_text_init(&runtext, WIDGET_ID_DEFAULT, "runarea", TEXT_COLOR_SELECT, TEXT_ALIGN_CENTER, "Start");
    widget_area_init(&uninstallarea, "uninstallarea", WIDGET_IN_DEFAULT, 4, 7, 4, 1);
    widget_text_init(&uninstalltext, WIDGET_ID_DEFAULT, "uninstallarea", TEXT_COLOR_DISABLE, TEXT_ALIGN_CENTER, "Uninstall");
    view_addselection(&view, &runarea);
    view_addselection(&view, &uninstallarea);
    view_init(&view, "app", load, 0, config, button);
    view_register(&view, &titlearea);
    view_register(&view, &titletext);
    view_register(&view, &descriptionarea);
    view_register(&view, &descriptiontext);
    view_register(&view, &runarea);
    view_register(&view, &runtext);
    view_register(&view, &uninstallarea);
    view_register(&view, &uninstalltext);
    main_registerview(&view);

}

