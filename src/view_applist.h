struct view_applist
{

    struct view base;
    struct view_app *appview;
    struct db_applist applist;
    struct menu menu;
    struct textbox emptytextbox;
    unsigned int (*onload)(struct db_applist *applist);

};

struct view_applist *view_applist_setup(unsigned int w, unsigned int h);
