#include <stdlib.h>
#include <stdio.h>
#include "define.h"
#include "box.h"
#include "list.h"
#include "widget.h"
#include "selection.h"
#include "view.h"
#include "db.h"
#include "main.h"

static struct view view;
static struct widget areaall;
static struct widget areainstalled;
static struct widget areanew;
static struct widget areaupdated;
static struct widget areasynchronize;
static struct widget textall;
static struct widget textallitems;
static struct widget textinstalled;
static struct widget textinstalleditems;
static struct widget textnew;
static struct widget textnewitems;
static struct widget textupdated;
static struct widget textupdateditems;
static struct widget textsynchronize;
static char all[16];
static char new[16];
static char updated[16];
static char installed[16];

static void button(unsigned int key)
{

    selection_move(&view.selection, key);

    if (key == KEY_A)
    {

        if (selection_isactive(&view.selection, &areaall))
            main_configview("applist", "list", "all");

        if (selection_isactive(&view.selection, &areainstalled))
            main_configview("applist", "list", "installed");

        if (selection_isactive(&view.selection, &areanew))
            main_configview("applist", "list", "new");

        if (selection_isactive(&view.selection, &areaupdated))
            main_configview("applist", "list", "updated");

        selection_select(&view.selection, key, "repolist", "applist");

    }

    if (selection_isactive(&view.selection, &areasynchronize))
        selection_select(&view.selection, key, "repolist", "sync");

    selection_unselect(&view.selection, key, "repolist");

}

static void load(void)
{

    snprintf(all, 16, "%u items", db_countapps());
    snprintf(new, 16, "%u items", db_countapps());
    snprintf(updated, 16, "%u items", db_countapps());
    snprintf(installed, 16, "%u items", db_countapps());
    selection_reset(&view.selection);

}

void view_repolist_setup(void)
{

    widget_area_init(&areaall, "areaall", "main", 0, 0, 8, 1);
    widget_area_init(&areainstalled, "areainstalled", "main", 0, 1, 8, 1);
    widget_area_init(&areanew, "areanew", "main", 0, 2, 8, 1);
    widget_area_init(&areaupdated, "areaupdated", "main", 0, 3, 8, 1);
    widget_area_init(&areasynchronize, "areasynchronize", "main", 0, 7, 8, 1);
    widget_text_init(&textall, "", "areaall", TEXT_COLOR_SELECT, TEXT_ALIGN_LEFT, "All");
    widget_text_init(&textallitems, "", "areaall", TEXT_COLOR_NORMAL, TEXT_ALIGN_RIGHT, all);
    widget_text_init(&textinstalled, "", "areainstalled", TEXT_COLOR_SELECT, TEXT_ALIGN_LEFT, "Installed");
    widget_text_init(&textinstalleditems, "", "areainstalled", TEXT_COLOR_NORMAL, TEXT_ALIGN_RIGHT, installed);
    widget_text_init(&textnew, "", "areanew", TEXT_COLOR_SELECT, TEXT_ALIGN_LEFT, "New");
    widget_text_init(&textnewitems, "", "areanew", TEXT_COLOR_NORMAL, TEXT_ALIGN_RIGHT, new);
    widget_text_init(&textupdated, "", "areaupdated", TEXT_COLOR_SELECT, TEXT_ALIGN_LEFT, "Updated");
    widget_text_init(&textupdateditems, "", "areaupdated", TEXT_COLOR_NORMAL, TEXT_ALIGN_RIGHT, updated);
    widget_text_init(&textsynchronize, "", "areasynchronize", TEXT_COLOR_SELECT, TEXT_ALIGN_CENTER, "Synchronize");
    selection_add(&view.selection, &areaall);
    selection_add(&view.selection, &areainstalled);
    selection_add(&view.selection, &areanew);
    selection_add(&view.selection, &areaupdated);
    selection_add(&view.selection, &areasynchronize);
    view_init(&view, "repolist", load, 0, 0, button);
    view_register(&view, &areaall);
    view_register(&view, &areainstalled);
    view_register(&view, &areanew);
    view_register(&view, &areaupdated);
    view_register(&view, &areasynchronize);
    view_register(&view, &textall);
    view_register(&view, &textallitems);
    view_register(&view, &textinstalled);
    view_register(&view, &textinstalleditems);
    view_register(&view, &textnew);
    view_register(&view, &textnewitems);
    view_register(&view, &textupdated);
    view_register(&view, &textupdateditems);
    view_register(&view, &textsynchronize);
    main_register(&view);

}

