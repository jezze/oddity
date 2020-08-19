#include <stdlib.h>
#include <stdio.h>
#include "define.h"
#include "box.h"
#include "list.h"
#include "view.h"
#include "db.h"
#include "main.h"
#include "widget.h"
#include "selection.h"

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
static struct selection selection;
static char all[16];
static char new[16];
static char updated[16];
static char installed[16];

static void place(struct box *size)
{

    widget_area_place(&areaall, size);
    widget_area_place(&areainstalled, size);
    widget_area_place(&areanew, size);
    widget_area_place(&areaupdated, size);
    widget_area_place(&areasynchronize, size);
    widget_text_place(&textall, &areaall.size);
    widget_text_place(&textallitems, &areaall.size);
    widget_text_place(&textinstalled, &areainstalled.size);
    widget_text_place(&textinstalleditems, &areainstalled.size);
    widget_text_place(&textnew, &areanew.size);
    widget_text_place(&textnewitems, &areanew.size);
    widget_text_place(&textupdated, &areaupdated.size);
    widget_text_place(&textupdateditems, &areaupdated.size);
    widget_text_place(&textsynchronize, &areasynchronize.size);

}

static void render(unsigned int ticks)
{

    selection_render(&selection, ticks);
    widget_text_render(&textall, ticks);
    widget_text_render(&textallitems, ticks);
    widget_text_render(&textinstalled, ticks);
    widget_text_render(&textinstalleditems, ticks);
    widget_text_render(&textnew, ticks);
    widget_text_render(&textnewitems, ticks);
    widget_text_render(&textupdated, ticks);
    widget_text_render(&textupdateditems, ticks);
    widget_text_render(&textsynchronize, ticks);

}

static void button(unsigned int key)
{

    selection_setclosest(&selection, key);

    if (key == KEY_A)
    {

        if (selection_isactive(&selection, &areaall))
            main_configview("applist", "list", "all");

        if (selection_isactive(&selection, &areainstalled))
            main_configview("applist", "list", "installed");

        if (selection_isactive(&selection, &areanew))
            main_configview("applist", "list", "new");

        if (selection_isactive(&selection, &areaupdated))
            main_configview("applist", "list", "updated");

        selection_select(&selection, key, "repolist", "applist");

    }

    if (selection_isactive(&selection, &areasynchronize))
        selection_select(&selection, key, "repolist", "sync");

    selection_unselect(&selection, key, "repolist");

}

static void load(void)
{

    snprintf(all, 16, "%u items", db_countapps());
    snprintf(new, 16, "%u items", db_countapps());
    snprintf(updated, 16, "%u items", db_countapps());
    snprintf(installed, 16, "%u items", db_countapps());
    main_setview(place, render, button);
    selection_reset(&selection);

}

void view_repolist_setup(void)
{

    view_init(&view, "repolist", load, 0);
    widget_area_init(&areaall, 0, 0, 8, 1);
    selection_add(&selection, &areaall);
    widget_area_init(&areainstalled, 0, 1, 8, 1);
    selection_add(&selection, &areainstalled);
    widget_area_init(&areanew, 0, 2, 8, 1);
    selection_add(&selection, &areanew);
    widget_area_init(&areaupdated, 0, 3, 8, 1);
    selection_add(&selection, &areaupdated);
    widget_area_init(&areasynchronize, 0, 7, 8, 1);
    selection_add(&selection, &areasynchronize);
    widget_text_init(&textall, TEXT_COLOR_SELECT, TEXT_ALIGN_LEFT, "All");
    widget_text_init(&textallitems, TEXT_COLOR_NORMAL, TEXT_ALIGN_RIGHT, all);
    widget_text_init(&textinstalled, TEXT_COLOR_SELECT, TEXT_ALIGN_LEFT, "Installed");
    widget_text_init(&textinstalleditems, TEXT_COLOR_NORMAL, TEXT_ALIGN_RIGHT, installed);
    widget_text_init(&textnew, TEXT_COLOR_SELECT, TEXT_ALIGN_LEFT, "New");
    widget_text_init(&textnewitems, TEXT_COLOR_NORMAL, TEXT_ALIGN_RIGHT, new);
    widget_text_init(&textupdated, TEXT_COLOR_SELECT, TEXT_ALIGN_LEFT, "Updated");
    widget_text_init(&textupdateditems, TEXT_COLOR_NORMAL, TEXT_ALIGN_RIGHT, updated);
    widget_text_init(&textsynchronize, TEXT_COLOR_SELECT, TEXT_ALIGN_CENTER, "Synchronize");
    main_register(&view);

}

