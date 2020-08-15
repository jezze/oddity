struct download
{

    char buffer[1024];
    unsigned int count;
    unsigned int totalbytes;
    unsigned int percentage;
    unsigned int timeremaining;

};

void download_run(void);
unsigned int download_parse(struct download *download, char *buffer, unsigned int count);
void download_create(struct download *download, unsigned int id, char *url, char *path, void (*ondata)(unsigned int id, void *data, unsigned int count), void (*oncomplete)(unsigned int id), void (*onfailure)(unsigned int id));
void download_init(struct download *download);
