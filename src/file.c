#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "file.h"

static char *home;

void file_init(void)
{

    char command[128];

    home = getenv("HOME");

    snprintf(command, 128, "mkdir -p %s/.oddity", home);
    system(command);

}

void file_getlocaldatabasepath(char *path, unsigned int count)
{

    snprintf(path, count, "%s/.oddity/local.db", home);

}

void file_getlocalremotedatabasepath(char *path, unsigned int count, unsigned int id)
{

    snprintf(path, count, "%s/.oddity/remote_%u.db", home, id);

}

void file_getlocalpackagepath(char *path, unsigned int count, char *sha1)
{

    snprintf(path, count, "/media/data/apps/%s.opk", sha1);

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

    char remotedatapath[128];

    file_getlocalremotedatabasepath(remotedatapath, 128, id);

    return file_remove(remotedatapath);

}

unsigned int file_removepackage(char *name)
{

    char remotedatapath[128];

    file_getlocalpackagepath(remotedatapath, 128, name);

    return file_remove(remotedatapath);

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

