struct view_repolist
{

    struct view base;
    struct view_applist *applistview;
    struct menu menu;
    struct box menubox;
    struct menuitem menuitems[4];

};

struct view_repolist *view_repolist_setup(unsigned int w, unsigned int h);
