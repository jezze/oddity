struct view_app
{

    struct view base;
    struct view_install *installview;
    struct view_uninstall *uninstallview;
    struct db_app *app;
    struct box titlebox;
    struct box shortbox;
    struct menu menu;
    struct box menubox;
    struct menuitem menuitems[3];

};

struct view_app *view_app_setup(unsigned int w, unsigned int h);
