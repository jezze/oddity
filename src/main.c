#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "box.h"
#include "text.h"
#include "list.h"
#include "menu.h"
#include "session.h"
#include "file.h"
#include "db.h"
#include "view.h"
#include "backend.h"
#include "main.h"

#define TIMELIMIT                       (1000 / 60)

void view_app_setup(void);
void view_applist_setup(void);
void view_front_setup(void);
void view_repolist_setup(void);
void view_settings_setup(void);
void view_settings_audio_setup(void);
void view_sync_setup(void);
static void (*_place)(struct box *size);
static void (*_render)(unsigned int ticks);
static void (*_button)(unsigned int key);
static unsigned int quit;
static unsigned int ticks;
static struct list views;

static void setup(void)
{

    view_app_setup();
    view_applist_setup();
    view_front_setup();
    view_repolist_setup();
    view_settings_setup();
    view_settings_audio_setup();
    view_sync_setup();

}

static void run(void)
{

    unsigned int frametime;

    while (!quit)
    {

        frametime = backend_ticks();

        session_poll();
        backend_pollevent(main_quit, _button);
        backend_render(ticks, _place, _render);

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
    backend_loadfont("habbo.ttf");
    backend_tilebackground("background");

}

static void destroy(void)
{

    backend_unloadfont();
    backend_unloadimage("background");
    backend_unloadsample("select");
    backend_unloadsample("unselect");
    backend_unloadsample("click");
    backend_destroy();

}

void main_setview(void (*place)(struct box *size), void (*render)(unsigned int ticks), void (*button)(unsigned int key))
{

    _place = place;
    _render = render;
    _button = button;

}

struct view *findview(char *name)
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

void main_register(struct view *view)
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

