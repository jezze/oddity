#include <stdlib.h>
#include <stdio.h>
#include "define.h"
#include "box.h"
#include "list.h"
#include "view.h"
#include "db.h"
#include "main.h"
#include "widget.h"
#include "selection.h"

static struct view view;
static struct widget_area areas[6];
static struct widget_text texts[10];
static struct selection selection;
static char all[16];
static char new[16];
static char updated[16];
static char installed[16];
static char last[32];
static struct db_applist applist;

static void place(unsigned int w, unsigned int h)
{

    widget_area_place(&areas[0], 0, 0, w, h);
    widget_area_place(&areas[1], 0, 0, w, h);
    widget_area_place(&areas[2], 0, 0, w, h);
    widget_area_place(&areas[3], 0, 0, w, h);
    widget_area_place(&areas[4], 0, 0, w, h);
    widget_area_place(&areas[5], 0, 0, w, h);
    widget_text_placein(&texts[0], &areas[0].size);
    widget_text_placein(&texts[1], &areas[0].size);
    widget_text_placein(&texts[2], &areas[1].size);
    widget_text_placein(&texts[3], &areas[1].size);
    widget_text_placein(&texts[4], &areas[2].size);
    widget_text_placein(&texts[5], &areas[2].size);
    widget_text_placein(&texts[6], &areas[3].size);
    widget_text_placein(&texts[7], &areas[3].size);
    widget_text_placein(&texts[8], &areas[4].size);
    widget_text_placein(&texts[9], &areas[5].size);

}

static void render(unsigned int ticks)
{

    if (selection.active)
        widget_area_render(selection.active->data);

    widget_text_render(&texts[0]);
    widget_text_render(&texts[1]);
    widget_text_render(&texts[2]);
    widget_text_render(&texts[3]);
    widget_text_render(&texts[4]);
    widget_text_render(&texts[5]);
    widget_text_render(&texts[6]);
    widget_text_render(&texts[7]);
    widget_text_render(&texts[8]);
    widget_text_render(&texts[9]);

}

static void button(unsigned int key)
{

    selection_setclosest(&selection, key);

    if (key == KEY_A && selection_isactive(&selection, &areas[0].item))
    {

        db_freeapps(&applist);
        db_loadapps(&applist);
        view_config("applist", "list", &applist);
        selection_select(&selection, key, "repolist", "applist");

    }

    if (key == KEY_A && selection_isactive(&selection, &areas[1].item))
    {

        db_freeapps(&applist);
        db_loadappswithstate(&applist, 3);
        view_config("applist", "list", &applist);
        selection_select(&selection, key, "repolist", "applist");

    }

    if (key == KEY_A && selection_isactive(&selection, &areas[2].item))
    {

        db_freeapps(&applist);
        db_loadappswithstate(&applist, 1);
        view_config("applist", "list", &applist);
        selection_select(&selection, key, "repolist", "applist");

    }

    if (key == KEY_A && selection_isactive(&selection, &areas[3].item))
    {

        db_freeapps(&applist);
        db_loadappswithstate(&applist, 2);
        view_config("applist", "list", &applist);
        selection_select(&selection, key, "repolist", "applist");

    }

    if (selection_isactive(&selection, &areas[4].item))
        selection_select(&selection, key, "repolist", "sync");

    selection_unselect(&selection, key, "repolist");

}

static void load(void)
{

    snprintf(all, 16, "%u items", db_countapps());
    snprintf(new, 16, "%u items", db_countappswithstate(1));
    snprintf(updated, 16, "%u items", db_countappswithstate(2));
    snprintf(installed, 16, "%u items", db_countappswithstate(3));
    snprintf(last, 32, "Last updated: %s", "Never");
    main_setview(place, render, button);

    selection.active = selection.list.head;

}

void view_repolist_setup(void)
{

    view_init(&view, "repolist", load, 0);
    view_register(&view);
    widget_area_init(&areas[0], 0, 0, 8, 1);
    selection_add(&selection, &areas[0].item);
    widget_area_init(&areas[1], 0, 1, 8, 1);
    selection_add(&selection, &areas[1].item);
    widget_area_init(&areas[2], 0, 2, 8, 1);
    selection_add(&selection, &areas[2].item);
    widget_area_init(&areas[3], 0, 3, 8, 1);
    selection_add(&selection, &areas[3].item);
    widget_area_init(&areas[4], 2, 6, 4, 1);
    selection_add(&selection, &areas[4].item);
    widget_area_init(&areas[5], 0, 7, 8, 1);
    widget_text_init(&texts[0], TEXT_COLOR_SELECT, TEXT_ALIGN_LEFT, "All");
    widget_text_init(&texts[1], TEXT_COLOR_NORMAL, TEXT_ALIGN_RIGHT, all);
    widget_text_init(&texts[2], TEXT_COLOR_SELECT, TEXT_ALIGN_LEFT, "Installed");
    widget_text_init(&texts[3], TEXT_COLOR_NORMAL, TEXT_ALIGN_RIGHT, installed);
    widget_text_init(&texts[4], TEXT_COLOR_SELECT, TEXT_ALIGN_LEFT, "New");
    widget_text_init(&texts[5], TEXT_COLOR_NORMAL, TEXT_ALIGN_RIGHT, new);
    widget_text_init(&texts[6], TEXT_COLOR_SELECT, TEXT_ALIGN_LEFT, "Updated");
    widget_text_init(&texts[7], TEXT_COLOR_NORMAL, TEXT_ALIGN_RIGHT, updated);
    widget_text_init(&texts[8], TEXT_COLOR_SELECT, TEXT_ALIGN_CENTER, "Synchronize");
    widget_text_init(&texts[9], TEXT_COLOR_NORMAL, TEXT_ALIGN_CENTER, last);

}

