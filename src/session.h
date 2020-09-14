struct session
{

    unsigned int state;
    unsigned int id;
    int fd[2];
    pid_t cpid;
    char *args[32];
    void (*ondata)(unsigned int id, void *data, unsigned int count);
    void (*oncomplete)(unsigned int id);
    void (*onfailure)(unsigned int id);

};

void session_poll(void);
void session_run(void);
struct session *session_create(unsigned int id, void (*ondata)(unsigned int id, void *data, unsigned int count), void (*oncomplete)(unsigned int id), void (*onfailure)(unsigned int id));
void session_setarg(struct session *session, unsigned int index, char *value);
