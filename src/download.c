#include <stdlib.h>
#include <string.h>
#include "session.h"
#include "download.h"

void download_run(void)
{

    session_run();

}

unsigned int download_parse(struct download *download, char *buffer, unsigned int count)
{

    unsigned int offset = 0;
    unsigned int i;

    for (i = 0; i < count; i++)
    {

        if (buffer[i] == '\n')
        {

            if (i - offset > 0)
            {

                char *end;

                download->totalbytes = strtol(buffer + offset, &end, 10);
                download->percentage = strtol(end + 1, 0, 10);

            }

            offset = i + 1;

        }

    }

    if (count > offset)
    {

        memcpy(buffer, buffer + offset, count - offset);

        count = count - offset;

    }

    return count;

}

void download_create(struct download *download, unsigned int id, char *url, char *path, void (*ondata)(unsigned int id, void *data, unsigned int count), void (*oncomplete)(unsigned int id), void (*onfailure)(unsigned int id))
{

    session_create("download", id, ondata, oncomplete, onfailure);
    session_setarg("download", 0, "./helper.sh");
    session_setarg("download", 1, "download");
    session_setarg("download", 2, url);
    session_setarg("download", 3, path);
    session_setarg("download", 4, 0);

}

void download_init(struct download *download)
{

    download->count = 0;
    download->totalbytes = 0;
    download->percentage = 0;
    download->timeremaining = 0;

}

