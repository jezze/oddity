void session_poll(void);
void session_run(void);
void session_create(char *name, unsigned int id, void (*ondata)(unsigned int id, void *data, unsigned int count), void (*oncomplete)(unsigned int id));
void session_setarg(char *name, unsigned int index, char *value);
