#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "config.h"
#include "list.h"
#include "session.h"

static struct session sessions[MAXSESSIONS];
static struct list avail;
static struct list ready;
static struct list running;

static int spawn(struct session *session)
{

    if (pipe2(session->fd, O_NONBLOCK) == -1)
        return -1;

    session->cpid = fork();

    if (session->cpid == -1)
        return -1;

    if (session->cpid == 0)
    {

        close(session->fd[0]);
        dup2(session->fd[1], 1);

        if (execvp(session->args[0], session->args) == -1)
            exit(EXIT_FAILURE);

    }

    else
    {

        close(session->fd[1]);

        return 0;

    }

    return -1;

}

void session_poll(void)
{

    struct list_item *item = list_pickhead(&running);
    struct session *session;
    struct timeval val;
    char buffer[1024];
    int maxfd = 0;
    int count;
    fd_set rfds;

    if (!item)
        return;

    val.tv_sec = 0;
    val.tv_usec = 1;

    FD_ZERO(&rfds);

    session = item->data;

    FD_SET(session->fd[0], &rfds);

    if (session->fd[0] > maxfd)
        maxfd = session->fd[0];

    if (maxfd == 0)
    {

        list_add(&running, item);

        return;

    }

    select(maxfd + 1, &rfds, NULL, NULL, &val);

    if (!FD_ISSET(session->fd[0], &rfds))
    {

        list_add(&running, item);

        return;

    }

    count = read(session->fd[0], buffer, 1024);

    if (count > 0)
    {

        if (session->ondata)
            session->ondata(session->id, buffer, count);

        list_add(&running, item);

    }

    else
    {

        int status;

        waitpid(session->cpid, &status, WNOHANG);

        if (WIFEXITED(status))
        {

            close(session->fd[0]);

            if (WEXITSTATUS(status) == 0)
            {

                if (session->oncomplete)
                    session->oncomplete(session->id);

            }

            else
            {

                if (session->onfailure)
                    session->onfailure(session->id);

            }

            list_add(&avail, item);

        }

    }

}

void session_run(void)
{

    struct list_item *item = list_pickhead(&ready);
    struct session *session;

    if (item)
    {

        session = item->data;

        if (spawn(session) == 0)
            list_add(&running, item);
        else
            list_add(&avail, item);

    }

}

struct session *session_create(unsigned int id, void (*ondata)(unsigned int id, void *data, unsigned int count), void (*oncomplete)(unsigned int id), void (*onfailure)(unsigned int id))
{

    struct list_item *item = list_pickhead(&avail);
    struct session *session = 0;

    if (item)
    {

        session = item->data;
        session->id = id;
        session->ondata = ondata;
        session->oncomplete = oncomplete;
        session->onfailure = onfailure;

        list_add(&ready, item);

    }

    return session;

}

void session_setarg(struct session *session, unsigned int index, char *value)
{

    session->args[index] = value;

}

void session_setup(void)
{

    unsigned int i;

    for (i = 0; i < MAXSESSIONS; i++)
    {

        list_inititem(&sessions[i].item, &sessions[i]);
        list_add(&avail, &sessions[i].item);

    }

}

