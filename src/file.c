#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "file.h"

static char *home;

void file_init()
{

    FILE *file;
    char command[128];

    home = getenv("HOME");

    snprintf(command, 128, "mkdir -p %s/.ztore", home);

    file = popen(command, "r");

    pclose(file);

}

void file_getpath(char *path, char *file)
{

    snprintf(path, 256, "%s/.ztore/%s", home, file);

}

unsigned int file_copy(char *from, char *to)
{

    FILE *file;
    char command[128];

    snprintf(command, 128, "cp %s %s/.ztore/%s", from, home, to);

    file = popen(command, "r");

    if (!file)
        return 0;

    pclose(file);

    return 1;

}

unsigned int file_matchsha1(char *filename, char *sha1)
{

    FILE *file;
    char command[128];
    char data[40];
    size_t count;

    snprintf(command, 128, "sha1sum /media/data/apps/%s", filename);

    file = popen(command, "r");

    if (!file)
        return 0;

    count = fread(data, 1, 40, file);

    pclose(file);

    return (count == 40) ? !memcmp(data, sha1, 40) : 0;

}

