#define RENDER_PADDING                  12
#define RENDER_ROWHEIGHT                24

int render_getascent();
void render_getmetrics(char c, int *minx, int *maxx, int *miny, int *maxy, int *advance);
void render_background();
void render_glyph(char c, unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int color);
void render_rect(int x, int y, int w, int h);
void render_waitevent(struct view *view);
void render_update(struct view *view);
void render_initfont();
void render_init();
void render_destroyfont();
void render_destroy();
