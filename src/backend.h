#define RENDER_PADDING                  12
#define RENDER_ROWHEIGHT                24
#define KEY_LEFT                        1
#define KEY_RIGHT                       2
#define KEY_UP                          3
#define KEY_DOWN                        4
#define KEY_A                           5
#define KEY_B                           6
#define KEY_X                           7
#define KEY_Y                           8
#define KEY_L                           9
#define KEY_R                           10
#define KEY_SELECT                      11
#define KEY_START                       12
#define KEY_LOCKDOWN                    13
#define KEY_LOCKUP                      14

int backend_getascent();
void backend_getmetrics(char c, int *minx, int *maxx, int *miny, int *maxy, int *advance);
void backend_glyph(char c, unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int color);
void backend_rect(int x, int y, int w, int h);
void backend_waitevent(struct view *view);
void backend_update(struct view *view);
void backend_init();
void backend_destroy();
