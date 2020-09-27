void main_configview(const char *name, const char *key, const void *value);
void main_registerview(struct view *view);
void main_exec(const char id[6], const char *sha1);
void main_loadview(struct view *view, const char *match, const char *to);
void main_unloadview(struct view *view);
void main_goprev(const struct view *view, const char *id);
void main_gonext(const struct view *view, const char *id);
void main_button(unsigned int button);
void main_quit(void);
