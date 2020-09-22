#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include "config.h"
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
void view_settings_date_setup(void);
void view_settings_datetime_setup(void);
void view_settings_system_setup(void);
void view_settings_time_setup(void);
void view_sync_setup(void);
void view_sysinfo_setup(void);
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
    view_settings_date_setup();
    view_settings_datetime_setup();
    view_settings_system_setup();
    view_settings_time_setup();
    view_sync_setup();
    view_sysinfo_setup();

}

static void run(void)
{

    unsigned int frametime;

    while (!quit)
    {

        frametime = backend_ticks();

        session_run();
        session_poll();
        backend_pollevent(main_quit, active->onbutton);
        view_precheck(active);

        if (active->onstep)
            active->onstep(ticks);

        backend_render(active, ticks);

        if (backend_ticks() - frametime < TIMELIMIT)
            backend_delay(TIMELIMIT - (backend_ticks() - frametime));

        ticks++;

    }

}

static void init(void)
{

    backend_init();
    backend_loadimage("background", "back.png");
    backend_createbackground("background");
    backend_loadfont("default", 16, "habbo.ttf");
    backend_loadfont("icon", 16, "icofont.ttf");
    backend_loadfont("bigicon", 40, "icofont.ttf");
    backend_loadsample("select", "select.wav");
    backend_loadsample("unselect", "unselect.wav");
    backend_loadsample("click", "click.wav");

}

static void destroy(void)
{

    backend_unloadsample("select");
    backend_unloadsample("unselect");
    backend_unloadsample("click");
    backend_unloadfont("default");
    backend_unloadfont("icon");
    backend_unloadfont("bigicon");
    backend_destroybackground();
    backend_unloadimage("background");
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

    if (view->onconfig)
        view->onconfig(key, value);

}

static void loadview(struct view *view, char *parentname, unsigned int type)
{

    view->parentname = parentname;

    if (view->onload)
        view->onload(type);

    active = view;

}

void main_initview(char *name, char *parentname)
{

    struct view *view = findview(name);

    if (!view)
        return;

    loadview(view, parentname, VIEW_LOADTYPE_INIT);
    view_reset(view);

}

void main_destroyview(char *name)
{

    struct view *view = findview(name);

    if (!view)
        return;

    if (view->parentname)
    {

        struct view *parent = findview(view->parentname);

        if (!parent)
            return;

        loadview(parent, parent->parentname, VIEW_LOADTYPE_RESTORE);

    }

}

void main_registerview(struct view *view)
{

    list_add(&views, &view->item);

}

void main_exec(char id[6], char *sha1)
{

    char name[14];
    char opk[64];
    char *argv[3];
    unsigned int i;
    unsigned int k = 0;

    for (i = 0; i < 6; i++)
        name[k++] = tolower(id[i]);

    name[k++] = '.';

    for (i = 0; i < 7; i++)
        name[k++] = sha1[i];

    snprintf(opk, 64, "/media/data/apps/%s.opk", name);

    argv[0] = "opkrun";
    argv[1] = opk;
    argv[2] = 0;

    destroy();
    execvp(argv[0], argv);

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
    main_initview("front", 0);
    run();
    main_destroyview("front");
    destroy();

    return 0;

}

