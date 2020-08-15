struct session_progress
{

    char buffer[1024];
    unsigned int count;
    unsigned int totalbytes;
    unsigned int percentage;
    unsigned int timeremaining;

};

void session_poll(void);
void session_run(void);
void session_create(char *name, unsigned int id, void (*ondata)(unsigned int id, void *data, unsigned int count), void (*oncomplete)(unsigned int id));
void session_setarg(char *name, unsigned int index, char *value);
unsigned int session_parseprogress(struct session_progress *progress, char *buffer, unsigned int count);
void session_createprogress(struct session_progress *progress, unsigned int id, char *url, char *path, void (*ondata)(unsigned int id, void *data, unsigned int count), void (*oncomplete)(unsigned int id));
