#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "define.h"
#include "box.h"
#include "list.h"
#include "session.h"
#include "file.h"
#include "db.h"
#include "widget.h"
#include "view.h"
#include "backend.h"
#include "main.h"

void view_app_setup(void);
void view_applist_setup(void);
void view_exit_setup(void);
void view_front_setup(void);
void view_repolist_setup(void);
void view_settings_setup(void);
void view_settings_audio_setup(void);
void view_settings_system_setup(void);
void view_sync_setup(void);
static struct view *active;
static unsigned int quit;
static unsigned int ticks;
static struct list views;

static void setup(void)
{

    view_app_setup();
    view_applist_setup();
    view_exit_setup();
    view_front_setup();
    view_repolist_setup();
    view_settings_setup();
    view_settings_audio_setup();
    view_settings_system_setup();
    view_sync_setup();

}

static void run(void)
{

    unsigned int frametime;

    while (!quit)
    {

        frametime = backend_ticks();

        session_run();
        session_poll();
        backend_pollevent(main_quit, active->button);

        if (active->step)
            active->step(ticks);

        backend_render(active, ticks);

        if (backend_ticks() - frametime < TIMELIMIT)
            backend_delay(TIMELIMIT - (backend_ticks() - frametime));

        ticks++;

    }

}

static void init(void)
{

    backend_init();
    backend_loadsample("select", "select.wav");
    backend_loadsample("unselect", "unselect.wav");
    backend_loadsample("click", "click.wav");
    backend_loadimage("background", "back.png");
    backend_loadfont("default", 16, "habbo.ttf");
    backend_loadfont("icon", 16, "icofont.ttf");
    backend_loadfont("bigicon", 48, "icofont.ttf");
    backend_tilebackground("background");

}

static void destroy(void)
{

    backend_unloadimage("background");
    backend_unloadfont("default");
    backend_unloadfont("icon");
    backend_unloadfont("bigicon");
    backend_unloadsample("select");
    backend_unloadsample("unselect");
    backend_unloadsample("click");
    backend_destroy();

}

static struct view *findview(char *name)
{

    struct list_item *current;

    for (current = views.head; current; current = current->next)
    {

        struct view *view = current->data;

        if (!strcmp(view->name, name))
            return view;

    }

    return 0;

}

void main_configview(char *name, char *key, void *value)
{

    struct view *view = findview(name);

    if (!view)
        return;

    if (view->config)
        view->config(key, value);

}

void main_loadview(char *name, char *parentname)
{

    struct view *view = findview(name);

    if (!view)
        return;

    view->parentname = parentname;

    if (view->load)
        view->load();

    active = view;

}

void main_quitview(char *name)
{

    struct view *view = findview(name);

    if (!view)
        return;

    if (view->parentname)
    {

        struct view *parent = findview(view->parentname);

        if (!parent)
            return;

        main_loadview(view->parentname, parent->parentname);

    }

}

void main_registerview(struct view *view)
{

    list_add(&views, &view->item);

}

void main_exec(char *sha1)
{

    char *opkrun = "opkrun";
    char opk[64];
    char *argv[3];

    argv[0] = "opkrun";
    argv[1] = opk;
    argv[2] = 0;

    snprintf(opk, 64, "/media/data/apps/%s", sha1);
    destroy();
    execvp(opkrun, argv);

}

void main_quit(void)
{

    quit = 1;

}

int main(int argc, char **argv)
{

    init();
    setup();
    file_init();
    db_init();
    main_loadview("front", 0);
    run();
    main_quitview("front");
    destroy();

    return 0;

}

