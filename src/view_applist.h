struct view_applist
{

    struct view base;
    struct db_applist applist;

};

struct view_applist *view_applist_setup(unsigned int w, unsigned int h);
void view_applist_config(unsigned int offset);
