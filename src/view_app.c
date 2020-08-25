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

    selection_move(&view.selection, key);

    if (key == KEY_A)
    {

        if (selection_isactive(&view.selection, &runarea))
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

    selection_unselect(&view.selection, key, "app");

}

static void load(void)
{

    selection_reset(&view.selection);

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

    widget_area_init(&titlearea, "titlearea", "main", 0, 0, 8, 1);
    widget_text_init(&titletext, "", "titlearea", TEXT_COLOR_TITLE, TEXT_ALIGN_CENTER, 0);
    widget_area_init(&descriptionarea, "descriptionarea", "main", 0, 1, 8, 4);
    widget_text_init(&descriptiontext, "", "descriptionarea", TEXT_COLOR_NORMAL, TEXT_ALIGN_LEFT, 0);
    widget_area_init(&runarea, "runarea", "main", 0, 7, 4, 1);
    widget_text_init(&runtext, "", "runarea", TEXT_COLOR_SELECT, TEXT_ALIGN_CENTER, "Start");
    widget_area_init(&uninstallarea, "uninstallarea", "main", 4, 7, 4, 1);
    widget_text_init(&uninstalltext, "", "uninstallarea", TEXT_COLOR_DISABLE, TEXT_ALIGN_CENTER, "Uninstall");
    selection_add(&view.selection, &runarea);
    selection_add(&view.selection, &uninstallarea);
    view_init(&view, "app", load, 0, config, button);
    view_register(&view, &titlearea);
    view_register(&view, &titletext);
    view_register(&view, &descriptionarea);
    view_register(&view, &descriptiontext);
    view_register(&view, &runarea);
    view_register(&view, &runtext);
    view_register(&view, &uninstallarea);
    view_register(&view, &uninstalltext);
    main_register(&view);

}

