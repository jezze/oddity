#define VIEW_STATE_NONE                 0
#define VIEW_STATE_CONFIGURED           1
#define VIEW_STATE_RECONFIGURED         2
#define VIEW_STATE_DONE                 3

struct view
{

    void (*preload)(void);
    void (*load)(void);
    void (*render)(void);
    void (*keydown)(unsigned int key);
    void (*onquit)(void);

};

void view_quitloop(void);
void view_redraw(void);
void view_load(struct view *view);
void view_loop(void);
void view_setmode(struct view *view, void (*render)(void), void (*keydown)(unsigned int key));
void view_quit(struct view *view);
void view_init(struct view *view, void (*load)(void), void (*render)(void), void (*keydown)(unsigned int key));
