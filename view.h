struct view
{

    struct view *parent;
    void (*init)();
    void (*destroy)();
    void (*render)();
    void (*key)(unsigned int key);

};

void view_set(struct view *view);
void view_render();
void view_key(unsigned int keysym);
void view_init(struct view *view, void (*init)(), void (*destroy)(), void (*render)(), void (*key)(unsigned int keysym));
struct view *view_frontsetup(unsigned int w, unsigned int h, struct view *apps, struct view *browse);
struct view *view_appssetup(unsigned int w, unsigned int h);
struct view *view_browsesetup(unsigned int w, unsigned int h, struct view *showapp);
struct view *view_showappsetup(unsigned int w, unsigned int h);
