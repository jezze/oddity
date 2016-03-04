void file_init();
void file_getdatabasepath(char *path, unsigned int count);
void file_getremotedatabasepath(char *path, unsigned int count, unsigned int id);
void file_getpackagepath(char *path, unsigned int count, char *name);
unsigned int file_exist(char *path);
unsigned int file_copy(char *from, char *to);
unsigned int file_downloadremote(unsigned int id);
unsigned int file_matchsha1(char *path, char *checksum);
