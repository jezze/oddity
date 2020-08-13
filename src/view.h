#define VIEW_LOADTYPE_INIT              1
#define VIEW_LOADTYPE_RESTORE           2

struct view
{

    char *parentname;
    void (*load)(void);
    void (*config)(char *key, void *value);

};

void view_config(char *name, char *key, void *value);
void view_register(char *name, struct view *view);
struct view *view_find(char *name);
void view_load(char *name, char *parentname);
void view_quit(char *name);
void view_init(struct view *view, void (*load)(void), void (*config)(char *key, void *value));
