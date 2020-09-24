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
    void (*onbutton)(unsigned int button);

};

struct widget *view_findwidget(struct view *view, char *id);
void view_place(struct view *view);
void view_render(struct view *view, unsigned int ticks);
unsigned int view_isactive(struct view *view, char *id);
void view_reset(struct view *view);
void view_setattr(struct view *view, char *id, char *key, char *value);
void view_precheck(struct view *view);
void view_init(struct view *view, char *name, void (*onload)(unsigned int type), void (*onstep)(unsigned int ticks), void (*onconfig)(char *key, void *value), void (*onbutton)(unsigned int button));
