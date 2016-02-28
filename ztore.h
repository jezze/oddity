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

    void (*oninit)(unsigned int id);
    void (*ondestroy)();
    void (*onrender)();
    void (*onkey)(unsigned int key);

};

void view_set(struct view *view, unsigned int id);
void view_init(struct view *view, void (*oninit)(unsigned int id), void (*ondestroy)(), void (*onrender)(), void (*onkey)(unsigned int keysym));
void view_frontsetup();
void view_appssetup();
void view_browsesetup();
void view_showappsetup();
