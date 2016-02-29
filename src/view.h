struct view
{

    struct view *parent;
    void (*init)();
    void (*destroy)();
    void (*render)();
    void (*keydown)(unsigned int key);

};

void view_setparent(struct view *view, struct view *parent);
void view_init(struct view *view, void (*init)(), void (*destroy)(), void (*render)(), void (*keydown)(unsigned int key));
struct view *view_browsesetup(unsigned int w, unsigned int h, struct view *showapp);
struct view *view_categorysetup(unsigned int w, unsigned int h, struct view *browse);
struct view *view_frontsetup(unsigned int w, unsigned int h, struct view *category);
struct view *view_showappsetup(unsigned int w, unsigned int h);
void view_loadshowapp(unsigned int id);
