#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "define.h"
#include "box.h"
#include "text.h"
#include "menu.h"
#include "file.h"
#include "db.h"
#include "view.h"
#include "session.h"
#include "ztore.h"

struct progress
{

    char buffer[1024];
    unsigned int count;
    unsigned int totalbytes;
    unsigned int percentage;
    unsigned int timeremaining;

};

static struct view view;
static struct box statusbox;
static struct menu menu;
static struct box menubox;
static struct menuitem menuitems[1];
static struct progress progresses[8];
static struct db_remotelist remotelist;

static void place(unsigned int w, unsigned int h)
{

    box_setpartsize(&statusbox, w / 10, h / 10, 0, 0, 10, 8);
    box_setpartsize(&menubox, w / 10, h / 10, 0, 8, 10, 2);

}

static void render(void)
{

    char text[128];
    struct progress *progress = &progresses[0];

    snprintf(text, 128, "Progress: %d%%\nTotal bytes: %dKB", progress->percentage, progress->totalbytes);
    text_render(&statusbox, TEXT_COLOR_NORMAL, TEXT_ALIGN_LEFT, text);
    menu_render(&menu, &menubox);

}

static void button(unsigned int key)
{

    menu_button(&menu, key);

    switch (key)
    {

    case KEY_B:
        view_quit("sync");

        break;

    }

}

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

}

static void config(char *key, void *value)
{

}

static void menu_onselect(unsigned int index)
{

    switch (index)
    {

    case 0:
        /* Abort download */

        view_quit("sync");

        break;

    }

}

void view_sync_setup(void)
{

    view_init(&view, load, config);
    box_init(&statusbox);
    box_init(&menubox);
    menu_init(&menu, menuitems, 1, menu_onselect);
    menu_inititem(&menuitems[0], "Cancel", 0);
    menu_setrow(&menu, 0);
    view_register("sync", &view);

}

