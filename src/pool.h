void pool_area_create(struct view *view, char *id, char *in, int x, int y, int w, int h);
void pool_icon_create(struct view *view, char *id, char *in, unsigned int type);
void pool_option_create(struct view *view, char *id, char *in, char *name, char *value);
void pool_select_create(struct view *view, char *id, char *in);
void pool_slider_create(struct view *view, char *id, char *in, int min, int max, int value);
void pool_text_create(struct view *view, char *id, char *in, unsigned int type, unsigned int align, char *data);
void pool_toggle_create(struct view *view, char *id, char *in, unsigned int state);
