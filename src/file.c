#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "file.h"

static char *appspath = "/media/data/apps";

unsigned int file_mkdir(char *dir)
{

    FILE *file;
    char command[128];

    snprintf(command, 128, "mkdir %s", dir);

    file = popen(command, "r");

    if (!file)
        return 0;

    pclose(file);

    return 1;

}

unsigned int file_copy(char *from, char *to)
{

    FILE *file;
    char command[128];

    snprintf(command, 128, "cp %s %s", from, to);

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

    snprintf(command, 128, "sha1sum %s/%s", appspath, filename);

    file = popen(command, "r");

    if (!file)
        return 0;

    count = fread(data, 1, 40, file);

    pclose(file);

    return (count == 40) ? !memcmp(data, sha1, 40) : 0;

}

