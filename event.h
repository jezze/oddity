#define EVENT_TYPE_QUIT                 1
#define EVENT_TYPE_EXITVIEW             2
#define EVENT_TYPE_SHOWVIEW             3

struct event_quit
{

    struct view *caller;

};

struct event_exitview
{

    struct view *caller;
    unsigned int id;

};

struct event_showview
{

    struct view *caller;
    unsigned int id;

};

void event_quit(struct view *view);
void event_exitview(struct view *view, unsigned int id);
void event_showview(struct view *view, unsigned int id);
void event_register(void (*onevent)(unsigned int type, void *data));
