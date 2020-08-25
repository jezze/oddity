#include <stdlib.h>
#include "define.h"
#include "box.h"
#include "list.h"
#include "widget.h"
#include "selection.h"
#include "view.h"
#include "main.h"

static struct view quit;
static struct view view;
static struct widget libraryarea;
static struct widget librarytext;
static struct widget settingsarea;
static struct widget settingstext;
static struct widget exitarea;
static struct widget exittext;

static void button(unsigned int key)
{

    selection_move(&view.selection, key);
    selection_select(&view.selection, key, "libraryarea", view.name, "repolist");
    selection_select(&view.selection, key, "settingsarea", view.name, "settings");
    selection_select(&view.selection, key, "exitarea", view.name, "quit");

}

static void load(void)
{

    selection_reset(&view.selection);

}

void view_front_setup(void)
{

    widget_area_init(&libraryarea, "libraryarea", "main", 0, 4, 8, 1);
    widget_text_init(&librarytext, "", "libraryarea", TEXT_COLOR_SELECT, TEXT_ALIGN_CENTER, "Library");
    widget_area_init(&settingsarea, "settingsarea", "main", 0, 5, 8, 1);
    widget_text_init(&settingstext, "", "settingsarea", TEXT_COLOR_SELECT, TEXT_ALIGN_CENTER, "Settings");
    widget_area_init(&exitarea, "exitarea", "main", 0, 6, 8, 1);
    widget_text_init(&exittext, "", "exitarea", TEXT_COLOR_SELECT, TEXT_ALIGN_CENTER, "Exit");
    selection_add(&view.selection, &libraryarea);
    selection_add(&view.selection, &settingsarea);
    selection_add(&view.selection, &exitarea);
    view_init(&quit, "quit", main_quit, 0, 0, 0);
    main_register(&quit);
    view_init(&view, "front", load, 0, 0, button);
    view_register(&view, &libraryarea);
    view_register(&view, &librarytext);
    view_register(&view, &settingsarea);
    view_register(&view, &settingstext);
    view_register(&view, &exitarea);
    view_register(&view, &exittext);
    main_register(&view);

}

