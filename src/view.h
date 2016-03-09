#define VIEW_STATE_NONE                 0
#define VIEW_STATE_CONFIGURED           1
#define VIEW_STATE_RECONFIGURED         2
#define VIEW_STATE_DONE                 3

struct view
{

    void (*load)();
    void (*render)();
    void (*keydown)(unsigned int key);
    void (*onquit)();

};

void view_quitloop();
void view_redraw();
void view_load(struct view *view);
void view_loop();
void view_setmode(struct view *view, void (*render)(), void (*keydown)());
void view_quit(struct view *view);
void view_init(struct view *view, void (*load)(), void (*render)(), void (*keydown)(unsigned int key));
