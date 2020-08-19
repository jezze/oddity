#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "view.h"

void view_init(struct view *view, char *name, void (*load)(void), void (*config)(char *key, void *value))
{

    view->name = name;
    view->load = load;
    view->config = config;

    list_inititem(&view->item, view);

}

