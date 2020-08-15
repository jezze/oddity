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
static struct db_applist *applist;
static char *stateinfo[] = {"", "New", "Updated", "Installed"};
static unsigned int page;

static void place(unsigned int w, unsigned int h)
{

    if (applist->count)
    {

        widget_area_place(&areas[0], 0, 0, w, h);
        widget_area_place(&areas[1], 0, 0, w, h);
        widget_area_place(&areas[2], 0, 0, w, h);
        widget_area_place(&areas[3], 0, 0, w, h);
        widget_area_place(&areas[4], 0, 0, w, h);
        widget_area_place(&areas[5], 0, 0, w, h);
        widget_area_place(&areas[6], 0, 0, w, h);
        widget_area_place(&areas[7], 0, 0, w, h);
        widget_text_placein(&texts[0], &areas[0].size);
        widget_text_placein(&texts[1], &areas[0].size);
        widget_text_placein(&texts[2], &areas[1].size);
        widget_text_placein(&texts[3], &areas[1].size);
        widget_text_placein(&texts[4], &areas[2].size);
        widget_text_placein(&texts[5], &areas[2].size);
        widget_text_placein(&texts[6], &areas[3].size);
        widget_text_placein(&texts[7], &areas[3].size);
        widget_text_placein(&texts[8], &areas[4].size);
        widget_text_placein(&texts[9], &areas[4].size);
        widget_text_placein(&texts[10], &areas[5].size);
        widget_text_placein(&texts[11], &areas[5].size);
        widget_text_placein(&texts[12], &areas[6].size);
        widget_text_placein(&texts[13], &areas[6].size);
        widget_text_placein(&texts[14], &areas[7].size);
        widget_text_placein(&texts[15], &areas[7].size);

    }

    else
    {

        widget_area_place(&areas[8], 0, 0, w, h);
        widget_text_placein(&texts[16], &areas[8].size);

    }

}

static void render(unsigned int ticks)
{

    unsigned int start = page * 8;
    unsigned int max = applist->count - start;
    unsigned int i;

    if (max > 8)
        max = 8;

    if (max)
    {

        widget_area_render(selection.active->data);

        for (i = 0; i < max; i++)
        {

            texts[i * 2].data = applist->items[start + i].name;
            texts[i * 2 + 1].data = stateinfo[applist->items[start + i].state];

            widget_text_render(&texts[i * 2]);
            widget_text_render(&texts[i * 2 + 1]);

        }

    }

    else
    {

        widget_text_render(&texts[16]);

    }

}

static void button(unsigned int key)
{

    unsigned int maxpages = applist->count / 8;

    selection_setclosest(&selection, key);
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

    main_setview(place, render, button);

    selection.active = selection.list.head;

}

static void config(char *key, void *value)
{

    if (!strcmp(key, "list"))
        applist = value;

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

