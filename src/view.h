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
    struct selection selection;

};

void view_place(struct view *view, struct widget *parent);
void view_render(struct view *view, struct widget *parent, unsigned int ticks);
void view_register(struct view *view, struct widget *widget);
void view_init(struct view *view, char *name, void (*load)(void), void (*step)(unsigned int ticks), void (*config)(char *key, void *value), void (*button)(unsigned int key));
