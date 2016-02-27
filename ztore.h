#define SCREEN_WIDTH                    320
#define SCREEN_HEIGHT                   240
#define SCREEN_BPP                      24

struct box
{

    unsigned int x;
    unsigned int y;
    unsigned int w;
    unsigned int h;

};

struct view
{

    void (*init)();
    void (*destroy)();
    void (*render)();
    void (*handlekey)(unsigned int id);
    void (*handleevent)(unsigned int id);

};

void ztore_changestate(unsigned int state);
void view_handleevent(unsigned int index, unsigned int id);
void view_register(unsigned int index, void (*init)(), void (*destroy)(), void (*render)(), void (*handlekey)(unsigned int id), void (*handleevent)(unsigned int id));
void view_frontsetup();
void view_appssetup();
void view_browsesetup();
