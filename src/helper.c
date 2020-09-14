#include <stdlib.h>
#include <string.h>
#include "session.h"
#include "helper.h"

struct download
{

    char buffer[1024];
    unsigned int count;
    unsigned int totalbytes;
    unsigned int percentage;
    unsigned int timeremaining;
    unsigned int id;
    void (*onprogress)(unsigned int id, unsigned int percentage, unsigned int totalbytes);
    void (*oncomplete)(unsigned int id);
    void (*onfailure)(unsigned int id);

};

static struct download downloads[8];

static unsigned int downloadparse(struct download *download, char *buffer, unsigned int count)
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

static void downloadondata(unsigned int id, void *data, unsigned int count)
{

    struct download *download = &downloads[id];

    memcpy(download->buffer + download->count, data, count);

    download->count = downloadparse(download, download->buffer, download->count + count);

    download->onprogress(download->id, download->percentage, download->totalbytes);

}

static void downloadoncomplete(unsigned int id)
{

    struct download *download = &downloads[id];

    downloadondata(id, "\n", 1);

    download->oncomplete(download->id);

}

static void downloadonfailure(unsigned int id)
{

    struct download *download = &downloads[id];

    download->onfailure(download->id);

}

void helper_autostart_get(unsigned int id, void (*ondata)(unsigned int id, void *data, unsigned int count), void (*oncomplete)(unsigned int id), void (*onfailure)(unsigned int id))
{

    session_create("autostart_get", id, ondata, oncomplete, onfailure);
    session_setarg("autostart_get", id, 0, "./helper.sh");
    session_setarg("autostart_get", id, 1, "autostart_get");
    session_setarg("autostart_get", id, 2, 0);

}

void helper_autostart_install(unsigned int id, void (*ondata)(unsigned int id, void *data, unsigned int count), void (*oncomplete)(unsigned int id), void (*onfailure)(unsigned int id))
{

    session_create("autostart_install", id, ondata, oncomplete, onfailure);
    session_setarg("autostart_install", id, 0, "./helper.sh");
    session_setarg("autostart_install", id, 1, "autostart_install");
    session_setarg("autostart_install", id, 2, 0);

}

void helper_autostart_uninstall(unsigned int id, void (*ondata)(unsigned int id, void *data, unsigned int count), void (*oncomplete)(unsigned int id), void (*onfailure)(unsigned int id))
{

    session_create("autostart_uninstall", id, ondata, oncomplete, onfailure);
    session_setarg("autostart_uninstall", id, 0, "./helper.sh");
    session_setarg("autostart_uninstall", id, 1, "autostart_uninstall");
    session_setarg("autostart_uninstall", id, 2, 0);

}

void helper_date_get(unsigned int id, void (*ondata)(unsigned int id, void *data, unsigned int count), void (*oncomplete)(unsigned int id), void (*onfailure)(unsigned int id))
{

    session_create("date_get", id, ondata, oncomplete, onfailure);
    session_setarg("date_get", id, 0, "./helper.sh");
    session_setarg("date_get", id, 1, "date_get");
    session_setarg("date_get", id, 2, 0);

}

void helper_download(unsigned int id, char *url, char *path, void (*onprogress)(unsigned int id, unsigned int percentage, unsigned int totalbytes), void (*oncomplete)(unsigned int id), void (*onfailure)(unsigned int id))
{

    unsigned int index = 0; /* Allow multiple */
    struct download *download = &downloads[index];

    download->count = 0;
    download->totalbytes = 0;
    download->percentage = 0;
    download->timeremaining = 0;
    download->id = id;
    download->onprogress = onprogress;
    download->oncomplete = oncomplete;
    download->onfailure = onfailure;

    session_create("download", index, downloadondata, downloadoncomplete, downloadonfailure);
    session_setarg("download", index, 0, "./helper.sh");
    session_setarg("download", index, 1, "download");
    session_setarg("download", index, 2, url);
    session_setarg("download", index, 3, path);
    session_setarg("download", index, 4, 0);

}

void helper_time_get(unsigned int id, void (*ondata)(unsigned int id, void *data, unsigned int count), void (*oncomplete)(unsigned int id), void (*onfailure)(unsigned int id))
{

    session_create("time_get", id, ondata, oncomplete, onfailure);
    session_setarg("time_get", id, 0, "./helper.sh");
    session_setarg("time_get", id, 1, "time_get");
    session_setarg("time_get", id, 2, 0);

}

void helper_timezone_get(unsigned int id, void (*ondata)(unsigned int id, void *data, unsigned int count), void (*oncomplete)(unsigned int id), void (*onfailure)(unsigned int id))
{

    session_create("timezone_get", id, ondata, oncomplete, onfailure);
    session_setarg("timezone_get", id, 0, "./helper.sh");
    session_setarg("timezone_get", id, 1, "timezone_get");
    session_setarg("timezone_get", id, 2, 0);

}

void helper_volume_get(unsigned int id, char *channel, void (*ondata)(unsigned int id, void *data, unsigned int count), void (*oncomplete)(unsigned int id), void (*onfailure)(unsigned int id))
{

    session_create("volume_get", id, ondata, oncomplete, onfailure);
    session_setarg("volume_get", id, 0, "./helper.sh");
    session_setarg("volume_get", id, 1, "volume_get");
    session_setarg("volume_get", id, 2, channel);
    session_setarg("volume_get", id, 3, 0);

}

void helper_volume_increment(unsigned int id, char *channel, void (*ondata)(unsigned int id, void *data, unsigned int count), void (*oncomplete)(unsigned int id), void (*onfailure)(unsigned int id))
{

    session_create("volume_increment", id, ondata, oncomplete, onfailure);
    session_setarg("volume_increment", id, 0, "./helper.sh");
    session_setarg("volume_increment", id, 1, "volume_increment");
    session_setarg("volume_increment", id, 2, channel);
    session_setarg("volume_increment", id, 3, 0);

}

void helper_volume_decrement(unsigned int id, char *channel, void (*ondata)(unsigned int id, void *data, unsigned int count), void (*oncomplete)(unsigned int id), void (*onfailure)(unsigned int id))
{

    session_create("volume_decrement", id, ondata, oncomplete, onfailure);
    session_setarg("volume_decrement", id, 0, "./helper.sh");
    session_setarg("volume_decrement", id, 1, "volume_decrement");
    session_setarg("volume_decrement", id, 2, channel);
    session_setarg("volume_decrement", id, 3, 0);

}

