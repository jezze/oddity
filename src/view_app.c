#include <stdlib.h>
#include <string.h>
#include "define.h"
#include "box.h"
#include "list.h"
#include "view.h"
#include "main.h"
#include "widget.h"
#include "selection.h"
#include "db.h"

static struct view view;
static struct widget_area titlearea;
static struct widget_text titletext;
static struct widget_area descriptionarea;
static struct widget_text descriptiontext;
static struct widget_area runarea;
static struct widget_text runtext;
static struct widget_area uninstallarea;
static struct widget_text uninstalltext;
static struct selection selection;
static struct db_app app;

static void place(unsigned int w, unsigned int h)
{

    widget_area_place(&titlearea, 0, 0, w, h);
    widget_text_placein(&titletext, &titlearea.size);
    widget_area_place(&descriptionarea, 0, 0, w, h);
    widget_text_placein(&descriptiontext, &descriptionarea.size);
    widget_area_place(&runarea, 0, 0, w, h);
    widget_text_placein(&runtext, &runarea.size);
    widget_area_place(&uninstallarea, 0, 0, w, h);
    widget_text_placein(&uninstalltext, &uninstallarea.size);

}

static void render(unsigned int ticks)
{

    selection_render(&selection, ticks);
    widget_text_render(&titletext, ticks);
    widget_text_render(&descriptiontext, ticks);
    widget_text_render(&runtext, ticks);
    widget_text_render(&uninstalltext, ticks);

}

static void button(unsigned int key)
{

    selection_setclosest(&selection, key);

    if (key == KEY_A)
    {

        if (selection_isactive(&selection, &runarea.item))
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

    selection_unselect(&selection, key, "app");

}

static void load(void)
{

    main_setview(place, render, button);
    selection_reset(&selection);

}

static void config(char *key, void *value)
{

    if (!strcmp(key, "id"))
    {

        db_freeapp(&app);
        db_loadapp(&app, value);

        titletext.data = app.name;
        descriptiontext.data = app.shortdescription;

    }

}

void view_app_setup(void)
{

    view_init(&view, "app", load, config);
    view_register(&view);
    widget_area_init(&titlearea, 0, 0, 8, 1);
    widget_text_init(&titletext, TEXT_COLOR_TITLE, TEXT_ALIGN_CENTER, 0);
    widget_area_init(&descriptionarea, 0, 1, 8, 4);
    widget_text_init(&descriptiontext, TEXT_COLOR_NORMAL, TEXT_ALIGN_LEFT, 0);
    widget_area_init(&runarea, 0, 7, 4, 1);
    widget_text_init(&runtext, TEXT_COLOR_SELECT, TEXT_ALIGN_CENTER, "Start");
    selection_add(&selection, &runarea.item);
    widget_area_init(&uninstallarea, 4, 7, 4, 1);
    widget_text_init(&uninstalltext, TEXT_COLOR_DISABLE, TEXT_ALIGN_CENTER, "Uninstall");
    selection_add(&selection, &uninstallarea.item);

}

