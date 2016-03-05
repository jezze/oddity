#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "file.h"

static char *home;

void file_init()
{

    char command[128];

    home = getenv("HOME");

    snprintf(command, 128, "mkdir -p %s/.ztore", home);
    system(command);

}

void file_getdatabasepath(char *path, unsigned int count)
{

    snprintf(path, count, "%s/.ztore/data.db", home);

}

void file_getremotedatabasepath(char *path, unsigned int count, unsigned int id)
{

    snprintf(path, count, "%s/.ztore/remote_%u.db", home, id);

}

void file_getpackagepath(char *path, unsigned int count, char *name)
{

    snprintf(path, count, "/media/data/apps/%s", name);

}

unsigned int file_exist(char *path)
{

    return access(path, F_OK) != -1;

}

unsigned int file_copy(char *from, char *to)
{

    char command[128];

    snprintf(command, 128, "cp %s %s", from, to);

    return system(command) == 0;

}

unsigned int file_remove(char *name)
{

    char command[128];

    snprintf(command, 128, "rm %s", name);

    return system(command) == 0;

}

unsigned int file_removeremote(unsigned int id)
{

    char remotedatapath[64];

    file_getremotedatabasepath(remotedatapath, 64, id);

    return file_remove(remotedatapath);

}

unsigned int file_download(char *url, char *to)
{

    char command[128];

    snprintf(command, 128, "wget %s -O %s", url, to);

    return system(command) == 0;

}

unsigned int file_downloadremote(char *url, unsigned int id)
{

    char downloadpath[256];
    char remotedatapath[64];

    snprintf(downloadpath, 256, url);
    file_getremotedatabasepath(remotedatapath, 64, id);

    return file_download(downloadpath, remotedatapath);

}

unsigned int file_matchsha1(char *path, char *sha1)
{

    FILE *file;
    char command[128];
    char data[40];
    size_t count;

    snprintf(command, 128, "sha1sum %s", path);

    file = popen(command, "r");

    if (!file)
        return 0;

    count = fread(data, 1, 40, file);

    pclose(file);

    return (count == 40) ? !memcmp(data, sha1, 40) : 0;

}

