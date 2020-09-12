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
    struct widget *selected;

};

struct widget *view_findwidget(struct view *view, char *id);
struct widget *view_createwidget(struct view *view);
void view_place(struct view *view);
void view_render(struct view *view, unsigned int ticks);
unsigned int view_isactive(struct view *view, char *id);
void view_moveselection(struct view *view, unsigned int key);
void view_select(struct view *view, unsigned int key, char *match, char *from, char *to);
void view_unselect(struct view *view, unsigned int key, char *from);
void view_reset(struct view *view);
void view_setattr(struct view *view, char *id, char *key, char *value);
void view_init(struct view *view, char *name, void (*load)(void), void (*step)(unsigned int ticks), void (*config)(char *key, void *value), void (*button)(unsigned int key));
