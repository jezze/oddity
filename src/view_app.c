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
static struct widget_area title;
static struct widget_text titletext;
static struct widget_area description;
static struct widget_text descriptiontext;
static struct widget_area areas[3];
static struct widget_text texts[3];
static struct selection selection;
static struct db_app *app;

static void place(unsigned int w, unsigned int h)
{

    widget_area_place(&title, 0, 0, w, h);
    widget_text_placein(&titletext, &title.size);
    widget_area_place(&description, 0, 0, w, h);
    widget_text_placein(&descriptiontext, &description.size);
    widget_area_place(&areas[0], 0, 0, w, h);
    widget_area_place(&areas[1], 0, 0, w, h);
    widget_area_place(&areas[2], 0, 0, w, h);
    widget_text_placein(&texts[0], &areas[0].size);
    widget_text_placein(&texts[1], &areas[1].size);
    widget_text_placein(&texts[2], &areas[2].size);

}

static void render(unsigned int ticks)
{

    selection_render(&selection);
    widget_text_render(&titletext);
    widget_text_render(&descriptiontext);
    widget_text_render(&texts[0]);
    widget_text_render(&texts[1]);
    widget_text_render(&texts[2]);

}

/*
static void runpackage(void)
{

    struct db_packagelist packagelist;
    unsigned int i;

    db_loadpackagesfromapp(&packagelist, app);

    for (i = 0; i < packagelist.count; i++)
    {

        if (packagelist.items[i].state != 3)
            continue;

        main_exec(packagelist.items[i].name);

        break;

    }

    db_freepackages(&packagelist);

}
*/

static void button(unsigned int key)
{

    selection_setclosest(&selection, key);
    selection_unselect(&selection, key, "app");

}

static void load(void)
{

    main_setview(place, render, button);
    selection_reset(&selection);

}

static void config(char *key, void *value)
{

    if (!strcmp(key, "app"))
    {

        app = value;
        titletext.data = app->name;
        descriptiontext.data = app->shortdescription;

    }

}

void view_app_setup(void)
{

    view_init(&view, "app", load, config);
    view_register(&view);
    widget_area_init(&title, 0, 0, 8, 1);
    widget_text_init(&titletext, TEXT_COLOR_TITLE, TEXT_ALIGN_LEFT, 0);
    widget_area_init(&description, 0, 1, 8, 4);
    widget_text_init(&descriptiontext, TEXT_COLOR_NORMAL, TEXT_ALIGN_LEFT, 0);
    widget_area_init(&areas[0], 0, 5, 8, 1);
    selection_add(&selection, &areas[0].item);
    widget_area_init(&areas[1], 0, 6, 8, 1);
    selection_add(&selection, &areas[1].item);
    widget_area_init(&areas[2], 0, 7, 8, 1);
    selection_add(&selection, &areas[2].item);
    widget_text_init(&texts[0], TEXT_COLOR_SELECT, TEXT_ALIGN_LEFT, "Run");
    widget_text_init(&texts[1], TEXT_COLOR_SELECT, TEXT_ALIGN_LEFT, "Install");
    widget_text_init(&texts[2], TEXT_COLOR_SELECT, TEXT_ALIGN_LEFT, "Uninstall");

}

