struct view_sync
{

    struct view base;
    struct textbox status;
    struct box statusbox;
    struct menu menu;
    struct menuitem menuitems[1];
    unsigned int percentage;
    unsigned int totalbytes;
    unsigned int abortdownload;

};

struct view_sync *view_sync_setup(unsigned int w, unsigned int h);
