void main_setview(void (*place)(struct box *size), void (*render)(unsigned int ticks), void (*button)(unsigned int key));
void main_configview(char *name, char *key, void *value);
void main_loadview(char *name, char *parentname);
void main_quitview(char *name);
void main_register(struct view *view);
void main_exec(char *sha1);
void main_quit(void);
