#include <stdlib.h>
#include <stdio.h>
#include "define.h"
#include "box.h"
#include "list.h"
#include "widget.h"
#include "view.h"
#include "db.h"
#include "main.h"

static struct view view;
static char all[16];
static char new[16];
static char updated[16];
static char installed[16];

static void button(unsigned int key)
{

    view_moveselection(&view, key);

    if (key == KEY_A)
    {

        if (view_isactive(&view, "area_all"))
            main_configview("applist", "list", "all");

        if (view_isactive(&view, "area_installed"))
            main_configview("applist", "list", "installed");

        if (view_isactive(&view, "area_new"))
            main_configview("applist", "list", "new");

        if (view_isactive(&view, "area_updated"))
            main_configview("applist", "list", "updated");

    }

    view_select(&view, key, "area_all", view.name, "applist");
    view_select(&view, key, "area_installed", view.name, "applist");
    view_select(&view, key, "area_new", view.name, "applist");
    view_select(&view, key, "area_updated", view.name, "applist");
    view_select(&view, key, "area_synchronize", view.name, "sync");
    view_unselect(&view, key, view.name);

}

static void load(void)
{

    snprintf(all, 16, "%u items", db_countapps());
    snprintf(new, 16, "%u items", db_countapps());
    snprintf(updated, 16, "%u items", db_countapps());
    snprintf(installed, 16, "%u items", db_countapps());
    view_reset(&view);

}

void view_repolist_setup(void)
{

    widget_area_init(view_createwidget(&view), "area_all", WIDGET_IN_DEFAULT, 0, 0, 8, 1);
    widget_area_init(view_createwidget(&view), "area_installed", WIDGET_IN_DEFAULT, 0, 1, 8, 1);
    widget_area_init(view_createwidget(&view), "area_new", WIDGET_IN_DEFAULT, 0, 2, 8, 1);
    widget_area_init(view_createwidget(&view), "area_updated", WIDGET_IN_DEFAULT, 0, 3, 8, 1);
    widget_area_init(view_createwidget(&view), "area_synchronize", WIDGET_IN_DEFAULT, 0, 7, 8, 1);
    widget_text_init(view_createwidget(&view), WIDGET_ID_DEFAULT, "area_all", TEXT_COLOR_SELECT, TEXT_ALIGN_LEFT, "All");
    widget_text_init(view_createwidget(&view), WIDGET_ID_DEFAULT, "area_all", TEXT_COLOR_NORMAL, TEXT_ALIGN_RIGHT, all);
    widget_text_init(view_createwidget(&view), WIDGET_ID_DEFAULT, "area_installed", TEXT_COLOR_SELECT, TEXT_ALIGN_LEFT, "Installed");
    widget_text_init(view_createwidget(&view), WIDGET_ID_DEFAULT, "area_installed", TEXT_COLOR_NORMAL, TEXT_ALIGN_RIGHT, installed);
    widget_text_init(view_createwidget(&view), WIDGET_ID_DEFAULT, "area_new", TEXT_COLOR_SELECT, TEXT_ALIGN_LEFT, "New");
    widget_text_init(view_createwidget(&view), WIDGET_ID_DEFAULT, "area_new", TEXT_COLOR_NORMAL, TEXT_ALIGN_RIGHT, new);
    widget_text_init(view_createwidget(&view), WIDGET_ID_DEFAULT, "area_updated", TEXT_COLOR_SELECT, TEXT_ALIGN_LEFT, "Updated");
    widget_text_init(view_createwidget(&view), WIDGET_ID_DEFAULT, "area_updated", TEXT_COLOR_NORMAL, TEXT_ALIGN_RIGHT, updated);
    widget_text_init(view_createwidget(&view), WIDGET_ID_DEFAULT, "area_synchronize", TEXT_COLOR_SELECT, TEXT_ALIGN_CENTER, "Synchronize");
    view_addselection(&view, "area_all");
    view_addselection(&view, "area_installed");
    view_addselection(&view, "area_new");
    view_addselection(&view, "area_updated");
    view_addselection(&view, "area_synchronize");
    view_init(&view, "repolist", load, 0, 0, button);
    main_registerview(&view);

}

