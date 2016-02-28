#include <stdlib.h>
#include "view.h"
#include "event.h"

static void (*events[64])(unsigned int type, void *data);
static unsigned int eventcount;

static void raise(unsigned int type, void *data)
{

    unsigned int i;

    for (i = 0; i < eventcount; i++)
        events[i](type, data);

}

void event_quit(struct view *view)
{

    struct event_quit quit;

    quit.caller = view;

    raise(EVENT_TYPE_QUIT, &quit);

}

void event_exitview(struct view *view, unsigned int id)
{

    struct event_exitview exitview;

    exitview.caller = view;
    exitview.id = id;

    raise(EVENT_TYPE_EXITVIEW, &exitview);

}

void event_showview(struct view *view, unsigned int id)
{

    struct event_showview showview;

    showview.caller = view;
    showview.id = id;

    raise(EVENT_TYPE_SHOWVIEW, &showview);

}

void event_register(void (*onevent)(unsigned int type, void *data))
{

    events[eventcount] = onevent;
    eventcount++;

}

