#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "view.h"

static struct list views;

void view_config(char *name, char *key, void *value)
{

    struct view *view = view_find(name);

    if (!view)
        return;

    if (view->config)
        view->config(key, value);

}

void view_register(struct view *view)
{

    list_add(&views, &view->item);

}

struct view *view_find(char *name)
{

    struct list_item *current;

    for (current = views.head; current; current = current->next)
    {

        struct view *view = current->data;

        if (!strcmp(view->name, name))
            return view;

    }

    return 0;

}

void view_load(char *name, char *parentname)
{

    struct view *view = view_find(name);

    if (!view)
        return;

    view->parentname = parentname;

    if (view->load)
        view->load();

}

void view_quit(char *name)
{

    struct view *view = view_find(name);

    if (!view)
        return;

    if (view->parentname)
    {

        struct view *parent = view_find(view->parentname);

        if (!parent)
            return;

        view_load(view->parentname, parent->parentname);

    }

}

void view_init(struct view *view, char *name, void (*load)(void), void (*config)(char *key, void *value))
{

    view->name = name;
    view->load = load;
    view->config = config;

    list_inititem(&view->item, view);

}

