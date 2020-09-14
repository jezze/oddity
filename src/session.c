#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "config.h"
#include "session.h"

#define STATE_NONE 0
#define STATE_READY 1
#define STATE_RUNNING 2

static struct session sessions[MAXSESSIONS];

static struct session *findfree(void)
{

    unsigned int i;

    for (i = 0; i < MAXSESSIONS; i++)
    {

        struct session *session = &sessions[i];

        if (session->state == STATE_NONE)
            return session;

    }

    return 0;

}

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

    struct timeval val;
    unsigned int i;
    int maxfd = 0;
    fd_set rfds;

    val.tv_sec = 0;
    val.tv_usec = 1;

    FD_ZERO(&rfds);

    for (i = 0; i < MAXSESSIONS; i++)
    {

        struct session *session = &sessions[i];

        if (session->state != STATE_RUNNING)
            continue;

        FD_SET(session->fd[0], &rfds);

        if (session->fd[0] > maxfd)
            maxfd = session->fd[0];

    }

    if (maxfd == 0)
        return;

    select(maxfd + 1, &rfds, NULL, NULL, &val);

    for (i = 0; i < MAXSESSIONS; i++)
    {

        struct session *session = &sessions[i];
        char buffer[1024];
        int count;

        if (session->state != STATE_RUNNING)
            continue;

        if (!FD_ISSET(session->fd[0], &rfds))
            continue;

        count = read(session->fd[0], buffer, 1024);

        if (count > 0)
        {

            if (session->ondata)
                session->ondata(session->id, buffer, count);

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

                session->id = 0;
                session->state = STATE_NONE;

            }

        }

    }

}

void session_run(void)
{

    unsigned int i;

    for (i = 0; i < MAXSESSIONS; i++)
    {

        struct session *session = &sessions[i];
        int rc;

        if (session->state != STATE_READY)
            continue;

        rc = spawn(session);

        if (rc == 0)
        {

            session->state = STATE_RUNNING;

        }

        else
        {

            session->id = 0;
            session->state = STATE_NONE;

        }

    }

}

struct session *session_create(unsigned int id, void (*ondata)(unsigned int id, void *data, unsigned int count), void (*oncomplete)(unsigned int id), void (*onfailure)(unsigned int id))
{

    struct session *session = findfree();

    if (session)
    {

        session->state = STATE_READY;
        session->id = id;
        session->ondata = ondata;
        session->oncomplete = oncomplete;
        session->onfailure = onfailure;

    }

    return session;

}

void session_setarg(struct session *session, unsigned int index, char *value)
{

    session->args[index] = value;

}

