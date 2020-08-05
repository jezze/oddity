#define VIEW_STATE_NONE                 0
#define VIEW_STATE_CONFIGURED           1
#define VIEW_STATE_RECONFIGURED         2
#define VIEW_STATE_DONE                 3

struct view
{

    struct view *parent;
    void (*load)(void);

};

void view_load(struct view *view, struct view *parent);
void view_quit(struct view *view);
void view_init(struct view *view, void (*load)(void));
