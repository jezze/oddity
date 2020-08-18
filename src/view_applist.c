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
static struct widget_area areas[9];
static struct widget_text texts[17];
static struct selection selection;
static struct db_applist applist;
static unsigned int page;

static void place(struct box *size)
{

    widget_area_place(&areas[0], size);
    widget_area_place(&areas[1], size);
    widget_area_place(&areas[2], size);
    widget_area_place(&areas[3], size);
    widget_area_place(&areas[4], size);
    widget_area_place(&areas[5], size);
    widget_area_place(&areas[6], size);
    widget_area_place(&areas[7], size);
    widget_text_place(&texts[0], &areas[0].size);
    widget_text_place(&texts[1], &areas[0].size);
    widget_text_place(&texts[2], &areas[1].size);
    widget_text_place(&texts[3], &areas[1].size);
    widget_text_place(&texts[4], &areas[2].size);
    widget_text_place(&texts[5], &areas[2].size);
    widget_text_place(&texts[6], &areas[3].size);
    widget_text_place(&texts[7], &areas[3].size);
    widget_text_place(&texts[8], &areas[4].size);
    widget_text_place(&texts[9], &areas[4].size);
    widget_text_place(&texts[10], &areas[5].size);
    widget_text_place(&texts[11], &areas[5].size);
    widget_text_place(&texts[12], &areas[6].size);
    widget_text_place(&texts[13], &areas[6].size);
    widget_text_place(&texts[14], &areas[7].size);
    widget_text_place(&texts[15], &areas[7].size);
    widget_area_place(&areas[8], size);
    widget_text_place(&texts[16], &areas[8].size);

}

static void render(unsigned int ticks)
{

    char *stateinfo[] = {"", "New", "Updated", "Installed"};
    unsigned int start = page * 8;
    unsigned int max = applist.count - start;
    unsigned int i;

    if (max > 8)
        max = 8;

    if (max)
    {

        selection_render(&selection, ticks);

        for (i = 0; i < max; i++)
        {

            texts[i * 2].data = applist.items[start + i].name;
            texts[i * 2 + 1].data = stateinfo[1];

            widget_text_render(&texts[i * 2], ticks);
            widget_text_render(&texts[i * 2 + 1], ticks);

        }

    }

    else
    {

        widget_text_render(&texts[16], ticks);

    }

}

static void button(unsigned int key)
{

    unsigned int maxpages = applist.count / 8;
    unsigned int i;

    selection_setclosest(&selection, key);

    for (i = 0; i < 8; i++)
    {

        struct db_app *app = &applist.items[page * 8 + i];

        if (selection_isactive(&selection, &areas[i].item))
        {

            view_config("app", "id", app->id);
            selection_select(&selection, key, "applist", "app");

        }

    }

    selection_unselect(&selection, key, "applist");

    switch (key)
    {

    case KEY_LEFT:
        if (page > 0)
            page--;

        break;

    case KEY_RIGHT:
        if (page < maxpages)
            page++;

        break;

    }

}

static void load(void)
{

    page = 0;

    main_setview(place, render, button);
    selection_reset(&selection);

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

    view_init(&view, "applist", load, config);
    view_register(&view);
    widget_area_init(&areas[0], 0, 0, 8, 1);
    selection_add(&selection, &areas[0].item);
    widget_area_init(&areas[1], 0, 1, 8, 1);
    selection_add(&selection, &areas[1].item);
    widget_area_init(&areas[2], 0, 2, 8, 1);
    selection_add(&selection, &areas[2].item);
    widget_area_init(&areas[3], 0, 3, 8, 1);
    selection_add(&selection, &areas[3].item);
    widget_area_init(&areas[4], 0, 4, 8, 1);
    selection_add(&selection, &areas[4].item);
    widget_area_init(&areas[5], 0, 5, 8, 1);
    selection_add(&selection, &areas[5].item);
    widget_area_init(&areas[6], 0, 6, 8, 1);
    selection_add(&selection, &areas[6].item);
    widget_area_init(&areas[7], 0, 7, 8, 1);
    selection_add(&selection, &areas[7].item);
    widget_area_init(&areas[8], 0, 3, 8, 1);
    widget_text_init(&texts[0], TEXT_COLOR_SELECT, TEXT_ALIGN_LEFT, 0);
    widget_text_init(&texts[1], TEXT_COLOR_NORMAL, TEXT_ALIGN_RIGHT, 0);
    widget_text_init(&texts[2], TEXT_COLOR_SELECT, TEXT_ALIGN_LEFT, 0);
    widget_text_init(&texts[3], TEXT_COLOR_NORMAL, TEXT_ALIGN_RIGHT, 0);
    widget_text_init(&texts[4], TEXT_COLOR_SELECT, TEXT_ALIGN_LEFT, 0);
    widget_text_init(&texts[5], TEXT_COLOR_NORMAL, TEXT_ALIGN_RIGHT, 0);
    widget_text_init(&texts[6], TEXT_COLOR_SELECT, TEXT_ALIGN_LEFT, 0);
    widget_text_init(&texts[7], TEXT_COLOR_NORMAL, TEXT_ALIGN_RIGHT, 0);
    widget_text_init(&texts[8], TEXT_COLOR_SELECT, TEXT_ALIGN_LEFT, 0);
    widget_text_init(&texts[9], TEXT_COLOR_NORMAL, TEXT_ALIGN_RIGHT, 0);
    widget_text_init(&texts[10], TEXT_COLOR_SELECT, TEXT_ALIGN_LEFT, 0);
    widget_text_init(&texts[11], TEXT_COLOR_NORMAL, TEXT_ALIGN_RIGHT, 0);
    widget_text_init(&texts[12], TEXT_COLOR_SELECT, TEXT_ALIGN_LEFT, 0);
    widget_text_init(&texts[13], TEXT_COLOR_NORMAL, TEXT_ALIGN_RIGHT, 0);
    widget_text_init(&texts[14], TEXT_COLOR_SELECT, TEXT_ALIGN_LEFT, 0);
    widget_text_init(&texts[15], TEXT_COLOR_NORMAL, TEXT_ALIGN_RIGHT, 0);
    widget_text_init(&texts[16], TEXT_COLOR_NORMAL, TEXT_ALIGN_CENTER, "No items found.");

}

