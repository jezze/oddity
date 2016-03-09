struct view_app
{

    struct view base;
    struct view_install *installview;
    struct view_uninstall *uninstallview;
    struct db_app *app;
    struct textbox title;
    struct box titlebox;
    struct textbox shortdescription;
    struct box shortbox;
    struct menu menu;
    struct menuitem menuitems[3];
    void (*onload)();

};

struct view_app *view_app_setup(unsigned int w, unsigned int h);
