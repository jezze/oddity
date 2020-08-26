struct view
{

    char *name;
    char *parentname;
    struct list_item item;
    void (*load)(void);
    void (*step)(unsigned int ticks);
    void (*config)(char *key, void *value);
    void (*button)(unsigned int key);
    struct list widgets;
    struct widget main;
    struct { struct list list; struct widget *active; } selection;

};

void view_place(struct view *view, struct widget *parent);
void view_render(struct view *view, struct widget *parent, unsigned int ticks);
unsigned int view_isactive(struct view *view, char *id);
void view_moveselection(struct view *view, unsigned int key);
void view_select(struct view *view, unsigned int key, char *match, char *from, char *to);
void view_unselect(struct view *view, unsigned int key, char *from);
void view_addselection(struct view *view, struct widget *widget);
void view_renderselection(struct view *view, unsigned int ticks);
void view_reset(struct view *view);
void view_register(struct view *view, struct widget *widget);
void view_init(struct view *view, char *name, void (*load)(void), void (*step)(unsigned int ticks), void (*config)(char *key, void *value), void (*button)(unsigned int key));
