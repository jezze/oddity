struct view_install
{

    struct view base;
    struct db_app *app;
    unsigned int state;
    struct textbox status;
    struct menu menu;
    struct menuitem menuitems[1];
    void (*onload)();

};

struct view_install *view_install_setup(unsigned int w, unsigned int h);
