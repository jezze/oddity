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

    view_moveselection(&view, key);

    if (key == KEY_A)
    {

        if (view_isactive(&view, "areaall"))
            main_configview("applist", "list", "all");

        if (view_isactive(&view, "areainstalled"))
            main_configview("applist", "list", "installed");

        if (view_isactive(&view, "areanew"))
            main_configview("applist", "list", "new");

        if (view_isactive(&view, "areaupdated"))
            main_configview("applist", "list", "updated");

    }

    view_select(&view, key, "areaall", view.name, "applist");
    view_select(&view, key, "areainstalled", view.name, "applist");
    view_select(&view, key, "areanew", view.name, "applist");
    view_select(&view, key, "areaupdated", view.name, "applist");
    view_select(&view, key, "areasynchronize", view.name, "sync");
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

    widget_area_init(&areaall, "areaall", WIDGET_IN_DEFAULT, 0, 0, 8, 1);
    widget_area_init(&areainstalled, "areainstalled", WIDGET_IN_DEFAULT, 0, 1, 8, 1);
    widget_area_init(&areanew, "areanew", WIDGET_IN_DEFAULT, 0, 2, 8, 1);
    widget_area_init(&areaupdated, "areaupdated", WIDGET_IN_DEFAULT, 0, 3, 8, 1);
    widget_area_init(&areasynchronize, "areasynchronize", WIDGET_IN_DEFAULT, 0, 7, 8, 1);
    widget_text_init(&textall, WIDGET_ID_DEFAULT, "areaall", TEXT_COLOR_SELECT, TEXT_ALIGN_LEFT, "All");
    widget_text_init(&textallitems, WIDGET_ID_DEFAULT, "areaall", TEXT_COLOR_NORMAL, TEXT_ALIGN_RIGHT, all);
    widget_text_init(&textinstalled, WIDGET_ID_DEFAULT, "areainstalled", TEXT_COLOR_SELECT, TEXT_ALIGN_LEFT, "Installed");
    widget_text_init(&textinstalleditems, WIDGET_ID_DEFAULT, "areainstalled", TEXT_COLOR_NORMAL, TEXT_ALIGN_RIGHT, installed);
    widget_text_init(&textnew, WIDGET_ID_DEFAULT, "areanew", TEXT_COLOR_SELECT, TEXT_ALIGN_LEFT, "New");
    widget_text_init(&textnewitems, WIDGET_ID_DEFAULT, "areanew", TEXT_COLOR_NORMAL, TEXT_ALIGN_RIGHT, new);
    widget_text_init(&textupdated, WIDGET_ID_DEFAULT, "areaupdated", TEXT_COLOR_SELECT, TEXT_ALIGN_LEFT, "Updated");
    widget_text_init(&textupdateditems, WIDGET_ID_DEFAULT, "areaupdated", TEXT_COLOR_NORMAL, TEXT_ALIGN_RIGHT, updated);
    widget_text_init(&textsynchronize, WIDGET_ID_DEFAULT, "areasynchronize", TEXT_COLOR_SELECT, TEXT_ALIGN_CENTER, "Synchronize");
    view_addselection(&view, &areaall);
    view_addselection(&view, &areainstalled);
    view_addselection(&view, &areanew);
    view_addselection(&view, &areaupdated);
    view_addselection(&view, &areasynchronize);
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
    main_registerview(&view);

}

