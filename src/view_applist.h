struct view_applist
{

    struct view base;
    struct db_applist applist;
    void (*onload)(struct db_applist *applist);
    void (*onunload)(struct db_applist *applist);

};

struct view_applist *view_applist_setup(unsigned int w, unsigned int h);
