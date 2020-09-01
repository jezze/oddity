#include <stdlib.h>
#include "define.h"
#include "box.h"
#include "list.h"
#include "widget.h"
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

    view_moveselection(&view, key);
    view_select(&view, key, "area_library", view.name, "repolist");
    view_select(&view, key, "area_settings", view.name, "settings");
    view_select(&view, key, "area_exit", view.name, "quit");

}

static void load(void)
{

    view_reset(&view);

}

void view_front_setup(void)
{

    widget_area_init(&libraryarea, "area_library", WIDGET_IN_DEFAULT, 0, 4, 8, 1);
    widget_text_init(&librarytext, WIDGET_ID_DEFAULT, "area_library", TEXT_COLOR_SELECT, TEXT_ALIGN_CENTER, "Library");
    widget_area_init(&settingsarea, "area_settings", WIDGET_IN_DEFAULT, 0, 5, 8, 1);
    widget_text_init(&settingstext, WIDGET_ID_DEFAULT, "area_settings", TEXT_COLOR_SELECT, TEXT_ALIGN_CENTER, "Settings");
    widget_area_init(&exitarea, "area_exit", WIDGET_IN_DEFAULT, 0, 6, 8, 1);
    widget_text_init(&exittext, WIDGET_ID_DEFAULT, "area_exit", TEXT_COLOR_SELECT, TEXT_ALIGN_CENTER, "Exit");
    view_addselection(&view, &libraryarea);
    view_addselection(&view, &settingsarea);
    view_addselection(&view, &exitarea);
    view_init(&quit, "quit", main_quit, 0, 0, 0);
    main_registerview(&quit);
    view_init(&view, "front", load, 0, 0, button);
    view_register(&view, &libraryarea);
    view_register(&view, &librarytext);
    view_register(&view, &settingsarea);
    view_register(&view, &settingstext);
    view_register(&view, &exitarea);
    view_register(&view, &exittext);
    main_registerview(&view);

}

