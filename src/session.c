#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#define SESSION_MAX 32

#define STATE_NONE 0
#define STATE_READY 1
#define STATE_RUNNING 2
#define STATE_CLOSING 2

struct session
{

    unsigned int state;
    unsigned int id;
    int fd[2];
    pid_t cpid;
    char *name;
    char *args[32];
    void (*ondata)(unsigned int id, void *data, unsigned int count);
    void (*oncomplete)(unsigned int id);

};

static struct session sessions[SESSION_MAX];

static struct session *findfree(void)
{

    unsigned int i;

    for (i = 0; i < SESSION_MAX; i++)
    {

        if (sessions[i].state == STATE_NONE)
            return &sessions[i];

    }

    return 0;

}

static struct session *findname(char *name)
{

    unsigned int i;

    for (i = 0; i < SESSION_MAX; i++)
    {

        if (!strcmp(sessions[i].name, name))
            return &sessions[i];

    }

    return 0;

}

static int spawn(struct session *session)
{

    pid_t cpid;

    if (pipe2(session->fd, O_NONBLOCK) == -1)
        return -1;

    cpid = fork();

    if (cpid == -1)
        return -1;

    if (cpid == 0)
    {

        close(STDERR_FILENO);
        close(session->fd[0]);
        dup2(session->fd[1], 1);

        if (execvp(session->args[0], session->args) == -1)
            exit(EXIT_FAILURE);

    }

    else
    {

        close(session->fd[1]);

        session->cpid = cpid;

        return 0;

    }

    return -1;

}

void session_poll(void)
{

    char buf[1024];
    unsigned int i;
    int n, maxfd = 0;
    fd_set rfds;

    FD_ZERO(&rfds);

    for (i = 0; i < SESSION_MAX; i++)
    {

        struct session *session = &sessions[i];

        if (session->state != STATE_RUNNING)
            continue;

        if (session->fd[0] < 0)
            continue;

        FD_SET(session->fd[0], &rfds);

        if (session->fd[0] > maxfd)
            maxfd = session->fd[0];

    }

    if (maxfd == 0)
        return;

    select(maxfd + 1, &rfds, NULL, NULL, NULL);

    for (i = 0; i < SESSION_MAX; i++)
    {

        struct session *session = &sessions[i];

        if (session->state != STATE_RUNNING)
            continue;

        if (!FD_ISSET(session->fd[0], &rfds))
            continue;

        if ((n = read(session->fd[0], buf, 1024)) > 0)
        {

            session->ondata(session->id, buf, n);

        }

        else
        {

            session->state = STATE_CLOSING;

            close(session->fd[0]);

            session->fd[0] = -1;

            waitpid(session->cpid, NULL, 0);
            session->oncomplete(session->id);

            session->state = STATE_NONE;

        }

    }

}

void session_run(void)
{

    unsigned int i;

    for (i = 0; i < SESSION_MAX; i++)
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

            session->state = STATE_NONE;
            session->fd[0] = -1;

        }

    }

}

void session_create(char *name, unsigned int id, void (*ondata)(unsigned int id, void *data, unsigned int count), void (*oncomplete)(unsigned int id))
{

    struct session *session = findfree();

    if (session)
    {

        session->state = STATE_READY;
        session->name = name;
        session->id = id;
        session->ondata = ondata;
        session->oncomplete = oncomplete;

    }

}

void session_setarg(char *name, unsigned int index, char *value)
{

    struct session *session = findname(name);

    if (session)
        session->args[index] = value;

}

