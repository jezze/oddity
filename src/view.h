#define VIEW_LOADTYPE_INIT              1
#define VIEW_LOADTYPE_RESTORE           2

struct view
{

    char *name;
    char *parentname;
    struct list_item item;
    void (*load)(void);
    void (*config)(char *key, void *value);

};

void view_config(char *name, char *key, void *value);
void view_register(struct view *view);
struct view *view_find(char *name);
void view_load(char *name, char *parentname);
void view_quit(char *name);
void view_init(struct view *view, char *name, void (*load)(void), void (*config)(char *key, void *value));
