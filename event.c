#include <stdlib.h>
#include "ztore.h"
#include "event.h"

static void (*events[64])(unsigned int type, void *data);
static unsigned int eventcount;

static void raise(unsigned int type, void *data)
{

    unsigned int i;

    for (i = 0; i < eventcount; i++)
        events[i](type, data);

}

void event_quit()
{

    raise(EVENT_TYPE_QUIT, 0);

}

void event_exitview(unsigned int id)
{

    struct event_exitview exitview;

    exitview.id = id;

    raise(EVENT_TYPE_EXITVIEW, &exitview);

}

void event_showview(unsigned int id)
{

    struct event_showview showview;

    showview.id = id;

    raise(EVENT_TYPE_SHOWVIEW, &showview);

}

void event_register(void (*onevent)(unsigned int type, void *data))
{

    events[eventcount] = onevent;
    eventcount++;

}

