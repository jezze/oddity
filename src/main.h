void main_configview(char *name, char *key, void *value);
void main_registerview(struct view *view);
void main_exec(char id[6], char *sha1);
void main_loadview(struct view *view, char *match, char *to);
void main_unloadview(struct view *view);
void main_goprev(struct view *view, char *id);
void main_gonext(struct view *view, char *id);
void main_button(unsigned int button);
void main_quit(void);
