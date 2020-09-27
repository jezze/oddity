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
    void (*onconfig)(const char *key, const void *value);
    void (*onbutton)(unsigned int button);

};

struct widget *view_findwidget(const struct view *view, const char *id);
void view_place(struct view *view);
void view_render(const struct view *view, unsigned int ticks);
unsigned int view_isactive(const struct view *view, const char *id);
struct widget *view_getselectable(struct view *view);
void view_setattr(const struct view *view, const char *id, const char *key, char *value);
void view_init(struct view *view, char *name, void (*onload)(unsigned int type), void (*onstep)(unsigned int ticks), void (*onconfig)(const char *key, const void *value), void (*onbutton)(unsigned int button));
