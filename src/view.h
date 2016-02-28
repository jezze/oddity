struct view
{

    struct view *parent;
    void (*init)();
    void (*destroy)();
    void (*render)();
    void (*key)(unsigned int key);
    void (*load)(unsigned int id);

};

void view_setparent(struct view *view, struct view *parent);
void view_init(struct view *view, void (*init)(), void (*destroy)(), void (*render)(), void (*key)(unsigned int keysym), void (*load)(unsigned int id));
struct view *view_appssetup(unsigned int w, unsigned int h);
struct view *view_browsesetup(unsigned int w, unsigned int h, struct view *showapp);
struct view *view_categorysetup(unsigned int w, unsigned int h, struct view *browse);
struct view *view_frontsetup(unsigned int w, unsigned int h, struct view *apps, struct view *category);
struct view *view_showappsetup(unsigned int w, unsigned int h);
