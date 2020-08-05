void ztore_quit(void);
void ztore_setview(void (*render)(void), void (*keydown)(unsigned int key));
void ztore_redraw(struct view *view);
void ztore_exec(char *name);
