#define VIEW_STATE_NONE                 0
#define VIEW_STATE_CONFIGURED           1
#define VIEW_STATE_RECONFIGURED         2
#define VIEW_STATE_DONE                 3

struct view
{

    unsigned int state;
    void (*show)();
    void (*render)();
    void (*keydown)(unsigned int key);
    void (*onquit)();

};

void view_quit(struct view *view);
void view_init(struct view *view, void (*show)(), void (*render)(), void (*keydown)(unsigned int key));
struct view *view_app_setup(unsigned int w, unsigned int h);
void view_app_config(unsigned int id);
struct view *view_applist_setup(unsigned int w, unsigned int h, struct view *app);
void view_applist_config(unsigned int offset);
struct view *view_repolist_setup(unsigned int w, unsigned int h, struct view *applist);
struct view *view_sync_setup(unsigned int w, unsigned int h);
struct view *view_front_setup(unsigned int w, unsigned int h, struct view *repolist, struct view *sync);
