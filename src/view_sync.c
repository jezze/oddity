#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "define.h"
#include "box.h"
#include "text.h"
#include "list.h"
#include "view.h"
#include "ztore.h"
#include "widget.h"
#include "selection.h"
#include "file.h"
#include "db.h"
#include "session.h"

struct progress
{

    char buffer[1024];
    unsigned int count;
    unsigned int totalbytes;
    unsigned int percentage;
    unsigned int timeremaining;

};

static struct view view;
static struct progress progresses[8];
static struct widget_area areas[2];
static struct widget_text texts[2];
static struct selection selection;
static struct db_remotelist remotelist;
static char text[128];

static unsigned int parsedata(struct progress *progress, char *buffer, unsigned int count)
{

    unsigned int offset = 0;
    unsigned int i;

    for (i = 0; i < count; i++)
    {

        if (buffer[i] == '\n')
        {

            if (i - offset > 0)
            {

                char *end;

                progress->totalbytes = strtol(buffer + offset, &end, 10);
                progress->percentage = strtol(end + 1, 0, 10);

            }

            offset = i + 1;

        }

    }

    if (count > offset)
    {

        memcpy(buffer, buffer + offset, count - offset);

        count = count - offset;

    }

    return count;

}

static void ondata(unsigned int id, void *data, unsigned int count)
{

    struct progress *progress = &progresses[id];

    memcpy(progress->buffer + progress->count, data, count);

    progress->count = parsedata(progress, progress->buffer, progress->count + count);

}

static void oncomplete(unsigned int id)
{

    struct db_remote *remote = &remotelist.items[id];

    ondata(id, "\n", 1);
    db_sync(remote);
    file_removeremote(remote->id);

}

static void place(unsigned int w, unsigned int h)
{

    widget_area_place(&areas[0], 0, 0, w, h);
    widget_area_place(&areas[1], 0, 0, w, h);
    widget_text_placein(&texts[0], &areas[0].size);
    widget_text_placein(&texts[1], &areas[1].size);

}

static void render(unsigned int ticks)
{

    struct progress *progress = &progresses[0];

    snprintf(text, 128, "Progress: %d%%\nTotal bytes: %dKB", progress->percentage, progress->totalbytes);
    widget_area_render(selection.active->data);
    widget_text_render(&texts[0]);

    if (progress->percentage < 100)
        widget_text_render(&texts[1]);

}

static void button(unsigned int key)
{

    selection_setclosest(&selection, key);
    selection_unselect(&selection, key, "sync");

}

static void load(void)
{

    unsigned int i;

    ztore_setview(place, render, button);
    db_freeremotes(&remotelist);
    db_loadremotes(&remotelist);

    for (i = 0; i < remotelist.count; i++)
    {

        struct progress *progress = &progresses[i];
        struct db_remote *remote = &remotelist.items[i];
        char path[128];

        progress->count = 0;
        progress->totalbytes = 0;
        progress->percentage = 0;
        progress->timeremaining = 0;

        file_getremotedatabasepath(path, 128, remote->id);
        session_create("download1", i, ondata, oncomplete);
        session_setarg("download1", 0, "wget");
        session_setarg("download1", 1, "-q");
        session_setarg("download1", 2, "--show-progress");
        session_setarg("download1", 3, "--progress=dot");
        session_setarg("download1", 4, "-o");
        session_setarg("download1", 5, "/dev/stdout");
        session_setarg("download1", 6, remote->url);
        session_setarg("download1", 7, "-O");
        session_setarg("download1", 8, path);
        session_setarg("download1", 9, 0);

        break;

    }
 
    session_run();

    selection.active = selection.list.head;

}

void view_sync_setup(void)
{

    view_init(&view, "sync", load, 0);
    view_register(&view);
    widget_area_init(&areas[0], 0, 0, 8, 6);
    widget_area_init(&areas[1], 0, 7, 8, 1);
    widget_text_init(&texts[0], TEXT_COLOR_NORMAL, TEXT_ALIGN_LEFT, text);
    widget_text_init(&texts[1], TEXT_COLOR_SELECT, TEXT_ALIGN_LEFT, "Cancel");
    list_add(&selection.list, &areas[1].item);

}

