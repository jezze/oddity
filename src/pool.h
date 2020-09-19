void pool_create_area(struct view *view, char *id, char *in, int x, int y, int w, int h);
void pool_create_icon(struct view *view, char *id, char *in, unsigned int type);
void pool_create_option(struct view *view, char *id, char *in, char *name, char *value);
void pool_create_select(struct view *view, char *id, char *in, char *value);
void pool_create_slider(struct view *view, char *id, char *in, int min, int max, int value);
void pool_create_text(struct view *view, char *id, char *in, unsigned int type, unsigned int align, char *data);
void pool_create_toggle(struct view *view, char *id, char *in, unsigned int state);
