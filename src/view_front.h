struct view_front
{

    struct view base;
    struct view_repolist *repolistview;
    struct view_sync *syncview;
    struct box greetingbox;
    struct menu menu;
    struct box menubox;
    struct menuitem menuitems[4];

};

struct view_front *view_front_setup(unsigned int w, unsigned int h);
