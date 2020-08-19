#include <stdlib.h>
#include "define.h"
#include "box.h"
#include "list.h"
#include "view.h"
#include "main.h"
#include "widget.h"
#include "selection.h"

static struct view quit;
static struct view view;
static struct widget libraryarea;
static struct widget librarytext;
static struct widget settingsarea;
static struct widget settingstext;
static struct widget exitarea;
static struct widget exittext;
static struct selection selection;

static void place(struct box *size)
{

    widget_area_place(&libraryarea, size);
    widget_text_place(&librarytext, &libraryarea.size);
    widget_area_place(&settingsarea, size);
    widget_text_place(&settingstext, &settingsarea.size);
    widget_area_place(&exitarea, size);
    widget_text_place(&exittext, &exitarea.size);

}

static void render(unsigned int ticks)
{

    selection_render(&selection, ticks);
    widget_text_render(&librarytext, ticks);
    widget_text_render(&settingstext, ticks);
    widget_text_render(&exittext, ticks);

}

static void button(unsigned int key)
{

    selection_move(&selection, key);

    if (selection_isactive(&selection, &libraryarea))
        selection_select(&selection, key, "front", "repolist");

    if (selection_isactive(&selection, &settingsarea))
        selection_select(&selection, key, "front", "settings");

    if (selection_isactive(&selection, &exitarea))
        selection_select(&selection, key, "front", "quit");

}

static void load(void)
{

    main_setview(place, render, button);
    selection_reset(&selection);

}

void view_front_setup(void)
{

    view_init(&quit, "quit", main_quit, 0);
    main_register(&quit);
    view_init(&view, "front", load, 0);
    widget_area_init(&libraryarea, 0, 4, 8, 1);
    widget_text_init(&librarytext, TEXT_COLOR_SELECT, TEXT_ALIGN_CENTER, "Library");
    widget_area_init(&settingsarea, 0, 5, 8, 1);
    widget_text_init(&settingstext, TEXT_COLOR_SELECT, TEXT_ALIGN_CENTER, "Settings");
    widget_area_init(&exitarea, 0, 6, 8, 1);
    widget_text_init(&exittext, TEXT_COLOR_SELECT, TEXT_ALIGN_CENTER, "Exit");
    selection_add(&selection, &libraryarea);
    selection_add(&selection, &settingsarea);
    selection_add(&selection, &exitarea);
    main_register(&view);

}

