#define EVENT_TYPE_QUIT                 1
#define EVENT_TYPE_EXITVIEW             2
#define EVENT_TYPE_SHOWVIEW             3

struct event_exitview
{

    unsigned int id;

};

struct event_showview
{

    unsigned int id;

};

void event_quit();
void event_exitview(unsigned int id);
void event_showview(unsigned int id);
void event_register(void (*onevent)(unsigned int type, void *data));
