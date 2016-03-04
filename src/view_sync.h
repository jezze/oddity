struct view_sync
{

    struct view base;
    struct textbox status;

};

struct view_sync *view_sync_setup(unsigned int w, unsigned int h);
