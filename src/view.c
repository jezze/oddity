#include <stdlib.h>
#include <string.h>
#include "view.h"

struct viewlist
{

    char *name;
    struct view *view;

};

static struct viewlist views[64];
static unsigned int viewcount;

void view_send(char *name, char *key, void *value)
{

    struct view *view = view_find(name);

    view->event(key, value);

}

void view_register(char *name, struct view *view)
{

    views[viewcount].name = name;
    views[viewcount].view = view;

    viewcount++;

}

struct view *view_find(char *name)
{

    unsigned int i;

    for (i = 0; i < 64; i++)
    {

        if (!strcmp(views[i].name, name))
            return views[i].view;

    }

    return 0;

}

void view_load(char *name, char *parentname)
{

    struct view *view = view_find(name);

    view->parentname = parentname;

    if (view->load)
        view->load();

}

void view_quit(char *name)
{

    struct view *view = view_find(name);

    if (view->parentname)
    {

        struct view *parent = view_find(view->parentname);

        view_load(view->parentname, parent->parentname);

    }

}

void view_init(struct view *view, void (*load)(void), void (*event)(char *key, void *value))
{

    view->load = load;
    view->event = event;

}

