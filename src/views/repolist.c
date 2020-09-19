#include "include.h"

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

static void load(unsigned int type)
{

    snprintf(all, 16, "%u items", db_countapps());
    snprintf(new, 16, "%u items", db_countapps());
    snprintf(updated, 16, "%u items", db_countapps());
    snprintf(installed, 16, "%u items", db_countapps());
    view_setattr(&view, "area_all", "selectable", "true");
    view_setattr(&view, "area_installed", "selectable", "true");
    view_setattr(&view, "area_new", "selectable", "true");
    view_setattr(&view, "area_updated", "selectable", "true");
    view_setattr(&view, "area_synchronize", "selectable", "true");

}

void view_repolist_setup(void)
{

    view_init(&view, "repolist", load, 0, 0, button);
    pool_area_create(&view, "area_all", WIDGET_IN_DEFAULT, 0, 0, 12, 1);
    pool_area_create(&view, "area_installed", WIDGET_IN_DEFAULT, 0, 1, 12, 1);
    pool_area_create(&view, "area_new", WIDGET_IN_DEFAULT, 0, 2, 12, 1);
    pool_area_create(&view, "area_updated", WIDGET_IN_DEFAULT, 0, 3, 12, 1);
    pool_area_create(&view, "area_synchronize", WIDGET_IN_DEFAULT, 0, 7, 12, 1);
    pool_text_create(&view, WIDGET_ID_DEFAULT, "area_all", TEXT_TYPE_SELECT, TEXT_ALIGN_LEFT, "All");
    pool_text_create(&view, WIDGET_ID_DEFAULT, "area_all", TEXT_TYPE_NORMAL, TEXT_ALIGN_RIGHT, all);
    pool_text_create(&view, WIDGET_ID_DEFAULT, "area_installed", TEXT_TYPE_SELECT, TEXT_ALIGN_LEFT, "Installed");
    pool_text_create(&view, WIDGET_ID_DEFAULT, "area_installed", TEXT_TYPE_NORMAL, TEXT_ALIGN_RIGHT, installed);
    pool_text_create(&view, WIDGET_ID_DEFAULT, "area_new", TEXT_TYPE_SELECT, TEXT_ALIGN_LEFT, "New");
    pool_text_create(&view, WIDGET_ID_DEFAULT, "area_new", TEXT_TYPE_NORMAL, TEXT_ALIGN_RIGHT, new);
    pool_text_create(&view, WIDGET_ID_DEFAULT, "area_updated", TEXT_TYPE_SELECT, TEXT_ALIGN_LEFT, "Updated");
    pool_text_create(&view, WIDGET_ID_DEFAULT, "area_updated", TEXT_TYPE_NORMAL, TEXT_ALIGN_RIGHT, updated);
    pool_text_create(&view, WIDGET_ID_DEFAULT, "area_synchronize", TEXT_TYPE_SELECT, TEXT_ALIGN_CENTER, "Synchronize");
    main_registerview(&view);

}

