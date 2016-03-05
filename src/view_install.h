struct view_install
{

    struct view base;
    struct db_app app;
    struct db_packagelist packagelist;
    unsigned int state;
    struct textbox status;
    struct menu menu;
    struct menuitem menuitems[1];
    unsigned int (*onload)(struct db_app *app, struct db_packagelist *packagelist);

};

struct view_install *view_install_setup(unsigned int w, unsigned int h);
