struct view_repolist
{

    struct view base;
    struct view_applist *applistview;
    struct menu menu;
    struct menuitem menuitems[2];

};

struct view_repolist *view_repolist_setup(unsigned int w, unsigned int h);
