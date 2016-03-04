struct view_app
{

    struct view base;
    struct view_install *installview;
    struct db_app app;
    struct textbox title;
    struct textbox shortdescription;
    struct menu menu;
    struct menuitem menuitems[2];
    unsigned int (*onload)(struct db_app *app);

};

struct view_app *view_app_setup(unsigned int w, unsigned int h);
