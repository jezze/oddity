struct view_sync
{

    struct view base;
    unsigned int state;
    struct textbox status;
    struct menu menu;
    struct menuitem menuitems[1];

};

struct view_sync *view_sync_setup(unsigned int w, unsigned int h);
