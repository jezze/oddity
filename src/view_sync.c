#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "define.h"
#include "box.h"
#include "list.h"
#include "view.h"
#include "main.h"
#include "widget.h"
#include "selection.h"
#include "file.h"
#include "db.h"
#include "download.h"

static struct view view;
static struct download downloads[8];
static struct widget_area statusarea;
static struct widget_text statustext;
static struct widget_area cancelarea;
static struct widget_text canceltext;
static struct selection selection;
static struct db_remotelist remotelist;
static char text[128];

static void ondata(unsigned int id, void *data, unsigned int count)
{

    struct download *download = &downloads[id];

    memcpy(download->buffer + download->count, data, count);

    download->count = download_parse(download, download->buffer, download->count + count);

}

static void oncomplete(unsigned int id)
{

    struct db_remote *remote = &remotelist.items[id];

    ondata(id, "\n", 1);
    db_sync(remote);
    file_removeremote(remote->id);

}

static void onfailure(unsigned int id)
{

}

static void place(unsigned int w, unsigned int h)
{

    widget_area_place(&statusarea, 0, 0, w, h);
    widget_area_place(&cancelarea, 0, 0, w, h);
    widget_text_placein(&statustext, &statusarea.size);
    widget_text_placein(&canceltext, &cancelarea.size);

}

static void render(unsigned int ticks)
{

    struct download *download = &downloads[0];

    snprintf(text, 128, "Progress: %d%%\nTotal bytes: %dKB", download->percentage, download->totalbytes);
    selection_render(&selection, ticks);
    widget_text_render(&statustext, ticks);

    if (download->percentage < 100)
        widget_text_render(&canceltext, ticks);

}

static void button(unsigned int key)
{

    selection_setclosest(&selection, key);
    selection_unselect(&selection, key, "sync");

}

static void load(void)
{

    unsigned int i;

    db_freeremotes(&remotelist);
    db_loadremotes(&remotelist);

    for (i = 0; i < remotelist.count; i++)
    {

        char path[128];

        file_getremotedatabasepath(path, 128, remotelist.items[i].id);
        download_init(&downloads[i]);
        download_create(&downloads[i], i, remotelist.items[i].url, path, ondata, oncomplete, onfailure);

        break;

    }

    download_run();
    main_setview(place, render, button);
    selection_reset(&selection);

}

void view_sync_setup(void)
{

    view_init(&view, "sync", load, 0);
    view_register(&view);
    widget_area_init(&statusarea, 0, 0, 8, 6);
    widget_text_init(&statustext, TEXT_COLOR_NORMAL, TEXT_ALIGN_LEFT, text);
    widget_area_init(&cancelarea, 0, 7, 8, 1);
    widget_text_init(&canceltext, TEXT_COLOR_SELECT, TEXT_ALIGN_LEFT, "Cancel");
    selection_add(&selection, &cancelarea.item);

}

