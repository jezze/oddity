#include <stdlib.h>
#include "define.h"
#include "box.h"
#include "text.h"
#include "list.h"
#include "view.h"
#include "ztore.h"
#include "widget.h"
#include "selection.h"

static struct view quit;
static struct view view;
static struct widget_area areas[4];
static struct widget_text textbrowse;
static struct widget_text textsync;
static struct widget_text textsettings;
static struct widget_text textexit;
static struct selection selection;

static void place(unsigned int w, unsigned int h)
{

    widget_area_place(&areas[0], 0, 0, w, h);
    widget_area_place(&areas[1], 0, 0, w, h);
    widget_area_place(&areas[2], 0, 0, w, h);
    widget_area_place(&areas[3], 0, 0, w, h);
    widget_text_place(&textbrowse, areas[0].size.x, areas[0].size.y, areas[0].size.w, areas[0].size.h);
    widget_text_place(&textsync, areas[1].size.x, areas[1].size.y, areas[1].size.w, areas[1].size.h);
    widget_text_place(&textsettings, areas[2].size.x, areas[2].size.y, areas[2].size.w, areas[2].size.h);
    widget_text_place(&textexit, areas[3].size.x, areas[3].size.y, areas[3].size.w, areas[3].size.h);

}

static void render(unsigned int ticks)
{

    widget_area_render(selection.active->data);
    widget_text_render(&textbrowse);
    widget_text_render(&textsync);
    widget_text_render(&textsettings);
    widget_text_render(&textexit);

}

static void button(unsigned int key)
{

    selection_setclosest(&selection, key);
    selection_select(&selection, key, "front", &areas[0].item, "repolist");
    selection_select(&selection, key, "front", &areas[1].item, "sync");
    selection_select(&selection, key, "front", &areas[2].item, "settings");
    selection_select(&selection, key, "front", &areas[3].item, "quit");

}

static void load(void)
{

    ztore_setview(place, render, button);

    selection.active = selection.list.head;

}

void view_front_setup(void)
{

    view_init(&quit, "quit", ztore_quit, 0);
    view_register(&quit);
    view_init(&view, "front", load, 0);
    view_register(&view);
    widget_area_init(&areas[0], 0, 4, 8, 1);
    widget_area_init(&areas[1], 0, 5, 8, 1);
    widget_area_init(&areas[2], 0, 6, 8, 1);
    widget_area_init(&areas[3], 0, 7, 8, 1);
    widget_text_init(&textbrowse, TEXT_COLOR_SELECT, TEXT_ALIGN_LEFT, "Browse");
    widget_text_init(&textsync, TEXT_COLOR_SELECT, TEXT_ALIGN_LEFT, "Sync");
    widget_text_init(&textsettings, TEXT_COLOR_SELECT, TEXT_ALIGN_LEFT, "Settings");
    widget_text_init(&textexit, TEXT_COLOR_SELECT, TEXT_ALIGN_LEFT, "Exit");
    list_add(&selection.list, &areas[0].item);
    list_add(&selection.list, &areas[1].item);
    list_add(&selection.list, &areas[2].item);
    list_add(&selection.list, &areas[3].item);


}

