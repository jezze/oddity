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

void file_getremotepackagepath(char *path, unsigned int count, char *name)
{

    snprintf(path, count, "%s/%s/%s", "http://gcw.abstraction.se/", "apps/official", name);

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

    file_getremotedatabasepath(remotedatapath, 128, id);

    return file_remove(remotedatapath);

}

unsigned int file_removepackage(char *name)
{

    char remotedatapath[128];

    file_getpackagepath(remotedatapath, 128, name);

    return file_remove(remotedatapath);

}

unsigned int file_download(char *url, char *to, void (*notify)(unsigned int totalbytes, unsigned int percentage))
{

    FILE *fd;
    char command[256];
    char line[1024];
    unsigned int start = 0;

    snprintf(command, 256, "wget --progress=dot %s -O %s 2>&1", url, to);

    fd = popen(command, "r");

    if (!fd)
        return 0;

    while (fgets(line, 1024, fd))
    {

        if (strlen(line) == 0)
            return 0;

        if (strlen(line) == 1)
        {

            start = 1;

            break;

        }

    }

    if (!start)
        return 0;

    while (fgets(line, 1024, fd))
    {

        unsigned int totalbytes;
        unsigned int percentage;

        sscanf(line, "%u", &totalbytes);
        sscanf(line + 62, "%u", &percentage);

        if (notify)
            notify(totalbytes, percentage);

    }

    pclose(fd);

    return 1;

}

unsigned int file_downloadremote(char *url, unsigned int id, void (*notify)(unsigned int totalbytes, unsigned int percentage))
{

    char remotedatapath[128];

    file_getremotedatabasepath(remotedatapath, 128, id);

    return file_download(url, remotedatapath, notify);

}

unsigned int file_downloadpackage(char *name, void (*notify)(unsigned int totalbytes, unsigned int percentage))
{

    char packagepath[128];
    char remotepackagepath[128];

    file_getpackagepath(packagepath, 128, name);
    file_getremotepackagepath(remotepackagepath, 128, name);

    return file_download(remotepackagepath, packagepath, notify);

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

