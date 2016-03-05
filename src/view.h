#define VIEW_STATE_NONE                 0
#define VIEW_STATE_CONFIGURED           1
#define VIEW_STATE_RECONFIGURED         2
#define VIEW_STATE_DONE                 3

struct view
{

    unsigned int state;
    void (*setmode)(unsigned int mode);
    void (*load)();
    void (*render)();
    void (*keydown)(unsigned int key);
    void (*onquit)();

};

void view_quit(struct view *view);
void view_init(struct view *view, void (*setmode)(unsigned int mode), void (*load)(), void (*render)(), void (*keydown)(unsigned int key));
