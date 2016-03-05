struct view_uninstall
{

    struct view base;
    struct db_app *app;
    unsigned int state;
    struct textbox status;
    struct menu menu;
    struct menuitem menuitems[1];
    void (*onload)();

};

struct view_uninstall *view_uninstall_setup(unsigned int w, unsigned int h);
