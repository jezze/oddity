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
static struct widget_area areaall;
static struct widget_area areainstalled;
static struct widget_area areanew;
static struct widget_area areaupdated;
static struct widget_area areasynchronize;
static struct widget_text textall;
static struct widget_text textallitems;
static struct widget_text textinstalled;
static struct widget_text textinstalleditems;
static struct widget_text textnew;
static struct widget_text textnewitems;
static struct widget_text textupdated;
static struct widget_text textupdateditems;
static struct widget_text textsynchronize;
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

        if (selection_isactive(&selection, &areaall.item))
            view_config("applist", "list", "all");

        if (selection_isactive(&selection, &areainstalled.item))
            view_config("applist", "list", "installed");

        if (selection_isactive(&selection, &areanew.item))
            view_config("applist", "list", "new");

        if (selection_isactive(&selection, &areaupdated.item))
            view_config("applist", "list", "updated");

        selection_select(&selection, key, "repolist", "applist");

    }

    if (selection_isactive(&selection, &areasynchronize.item))
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
    view_register(&view);
    widget_area_init(&areaall, 0, 0, 8, 1);
    selection_add(&selection, &areaall.item);
    widget_area_init(&areainstalled, 0, 1, 8, 1);
    selection_add(&selection, &areainstalled.item);
    widget_area_init(&areanew, 0, 2, 8, 1);
    selection_add(&selection, &areanew.item);
    widget_area_init(&areaupdated, 0, 3, 8, 1);
    selection_add(&selection, &areaupdated.item);
    widget_area_init(&areasynchronize, 0, 7, 8, 1);
    selection_add(&selection, &areasynchronize.item);
    widget_text_init(&textall, TEXT_COLOR_SELECT, TEXT_ALIGN_LEFT, "All");
    widget_text_init(&textallitems, TEXT_COLOR_NORMAL, TEXT_ALIGN_RIGHT, all);
    widget_text_init(&textinstalled, TEXT_COLOR_SELECT, TEXT_ALIGN_LEFT, "Installed");
    widget_text_init(&textinstalleditems, TEXT_COLOR_NORMAL, TEXT_ALIGN_RIGHT, installed);
    widget_text_init(&textnew, TEXT_COLOR_SELECT, TEXT_ALIGN_LEFT, "New");
    widget_text_init(&textnewitems, TEXT_COLOR_NORMAL, TEXT_ALIGN_RIGHT, new);
    widget_text_init(&textupdated, TEXT_COLOR_SELECT, TEXT_ALIGN_LEFT, "Updated");
    widget_text_init(&textupdateditems, TEXT_COLOR_NORMAL, TEXT_ALIGN_RIGHT, updated);
    widget_text_init(&textsynchronize, TEXT_COLOR_SELECT, TEXT_ALIGN_CENTER, "Synchronize");

}

