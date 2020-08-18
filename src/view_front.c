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
static struct widget_area libraryarea;
static struct widget_text librarytext;
static struct widget_area settingsarea;
static struct widget_text settingstext;
static struct widget_area exitarea;
static struct widget_text exittext;
static struct selection selection;

static void place(unsigned int w, unsigned int h)
{

    widget_area_place(&libraryarea, 0, 0, w, h);
    widget_text_placein(&librarytext, &libraryarea.size);
    widget_area_place(&settingsarea, 0, 0, w, h);
    widget_text_placein(&settingstext, &settingsarea.size);
    widget_area_place(&exitarea, 0, 0, w, h);
    widget_text_placein(&exittext, &exitarea.size);

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

    selection_setclosest(&selection, key);

    if (selection_isactive(&selection, &libraryarea.item))
        selection_select(&selection, key, "front", "repolist");

    if (selection_isactive(&selection, &settingsarea.item))
        selection_select(&selection, key, "front", "settings");

    if (selection_isactive(&selection, &exitarea.item))
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
    view_register(&quit);
    view_init(&view, "front", load, 0);
    view_register(&view);
    widget_area_init(&libraryarea, 0, 3, 8, 1);
    widget_text_init(&librarytext, TEXT_COLOR_SELECT, TEXT_ALIGN_CENTER, "Library");
    selection_add(&selection, &libraryarea.item);
    widget_area_init(&settingsarea, 0, 4, 8, 1);
    widget_text_init(&settingstext, TEXT_COLOR_SELECT, TEXT_ALIGN_CENTER, "Settings");
    selection_add(&selection, &settingsarea.item);
    widget_area_init(&exitarea, 0, 5, 8, 1);
    widget_text_init(&exittext, TEXT_COLOR_SELECT, TEXT_ALIGN_CENTER, "Exit");
    selection_add(&selection, &exitarea.item);

}

