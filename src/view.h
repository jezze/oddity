struct view
{

    char *name;
    char *parentname;
    struct list_item item;
    struct list widgets;
    struct widget main;
    struct widget *selected;
    void (*onload)(unsigned int type);
    void (*onstep)(unsigned int ticks);
    void (*onconfig)(char *key, void *value);
    void (*onbutton)(unsigned int key);

};

struct widget *view_findwidget(struct view *view, char *id);
void view_place(struct view *view);
void view_render(struct view *view, unsigned int ticks);
unsigned int view_isactive(struct view *view, char *id);
void view_moveselection(struct view *view, unsigned int key);
void view_select(struct view *view, unsigned int key, char *match, char *to);
void view_unselect(struct view *view, unsigned int key);
void view_goprev(struct view *view, unsigned int key, char *id);
void view_gonext(struct view *view, unsigned int key, char *id);
void view_reset(struct view *view);
void view_setattr(struct view *view, char *id, char *key, char *value);
struct widget *view_widget_prev(struct view *view, struct widget *widget);
struct widget *view_widget_next(struct view *view, struct widget *widget);
struct widget *view_widget_prevchild(struct view *view, struct widget *widget, struct widget *parent);
struct widget *view_widget_nextchild(struct view *view, struct widget *widget, struct widget *parent);
void view_precheck(struct view *view);
void view_init(struct view *view, char *name, void (*onload)(unsigned int type), void (*onstep)(unsigned int ticks), void (*onconfig)(char *key, void *value), void (*onbutton)(unsigned int key));
