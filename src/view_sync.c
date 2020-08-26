#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "define.h"
#include "box.h"
#include "list.h"
#include "widget.h"
#include "selection.h"
#include "view.h"
#include "file.h"
#include "db.h"
#include "download.h"
#include "main.h"

static struct view view;
static struct download downloads[8];
static struct widget statusarea;
static struct widget statustext;
static struct db_remotelist remotelist;
static char text[128];

static void ondata(unsigned int id, void *data, unsigned int count)
{

    struct download *download = &downloads[id];

    memcpy(download->buffer + download->count, data, count);

    download->count = download_parse(download, download->buffer, download->count + count);

    snprintf(text, 128, "Downloading...\n\nProgress: %d%%\nTotal bytes: %dKB", download->percentage, download->totalbytes);

}

static void oncomplete(unsigned int id)
{

    struct db_remote *remote = &remotelist.items[id];
    struct download *download = &downloads[id];

    ondata(id, "\n", 1);
    db_sync(remote);
    file_removeremote(remote->id);
    snprintf(text, 128, "Synchronization complete!\n\nProgress: %d%%\nTotal bytes: %dKB", download->percentage, download->totalbytes);

}

static void onfailure(unsigned int id)
{

    struct download *download = &downloads[id];

    snprintf(text, 128, "Synchronization failed!\n\nProgress: %d%%\nTotal bytes: %dKB", download->percentage, download->totalbytes);

}

static void button(unsigned int key)
{

    selection_move(&view.selection, key);
    selection_unselect(&view.selection, key, view.name);

}

static void load(void)
{

    unsigned int i;

    snprintf(text, 128, "Connecting...\n\nProgress: %d%%\nTotal bytes: %dKB", 0, 0);
    db_freeremotes(&remotelist);
    db_loadremotes(&remotelist);

    for (i = 0; i < remotelist.count; i++)
    {

        char path[128];

        file_getlocalremotedatabasepath(path, 128, remotelist.items[i].id);
        download_init(&downloads[i]);
        download_create(&downloads[i], i, remotelist.items[i].urldatabase, path, ondata, oncomplete, onfailure);

        break;

    }

    download_run();
    selection_reset(&view.selection);

}

void view_sync_setup(void)
{

    widget_area_init(&statusarea, "statusarea", WIDGET_IN_DEFAULT, 0, 0, 8, 6);
    widget_text_init(&statustext, WIDGET_ID_DEFAULT, "statusarea", TEXT_COLOR_NORMAL, TEXT_ALIGN_LEFT, text);
    view_init(&view, "sync", load, 0, 0, button);
    view_register(&view, &statusarea);
    view_register(&view, &statustext);
    main_registerview(&view);

}

