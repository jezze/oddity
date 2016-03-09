struct view_uninstall
{

    struct view base;
    struct db_app *app;
    struct box statusbox;
    struct menu menu;
    struct box menubox;
    struct menuitem menuitems[1];
    void (*onload)();

};

struct view_uninstall *view_uninstall_setup(unsigned int w, unsigned int h);
