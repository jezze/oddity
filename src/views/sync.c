#include "include.h"

static struct view view;
static struct db_remotelist remotelist;
static char text[128];

static void onprogress(unsigned int id, unsigned int percentage, unsigned int totalbytes)
{

    snprintf(text, 128, "Downloading...\n\nProgress: %d%%\nTotal bytes: %dKB", percentage, totalbytes);

}

static void oncomplete(unsigned int id)
{

    struct db_remote *remote = &remotelist.items[id];

    db_sync(remote);
    file_removeremote(remote->id);
    snprintf(text, 128, "Synchronization complete!\n\nProgress: %d%%\nTotal bytes: %dKB", 100, 0);

}

static void onfailure(unsigned int id)
{

    snprintf(text, 128, "Synchronization failed!\n\nProgress: %d%%\nTotal bytes: %dKB", 0, 0);

}

static void button(unsigned int key)
{

    view_moveselection(&view, key);
    view_unselect(&view, key, view.name);

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
        helper_download(i, remotelist.items[i].urldatabase, path, onprogress, oncomplete, onfailure);

        break;

    }

    session_run();
    view_reset(&view);

}

void view_sync_setup(void)
{

    view_init(&view, "sync", load, 0, 0, button);
    widget_area_init(view_createwidget(&view), "area_status", WIDGET_IN_DEFAULT, 0, 0, 12, 6);
    widget_text_init(view_createwidget(&view), WIDGET_ID_DEFAULT, "area_status", TEXT_TYPE_NORMAL, TEXT_ALIGN_LEFT, text);
    main_registerview(&view);

}

