#include <stdlib.h>
#include <stdio.h>
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
void view_install_setup(void);
void view_repolist_setup(void);
void view_settings_setup(void);
void view_sync_setup(void);
void view_uninstall_setup(void);
static void (*_place)(unsigned int w, unsigned int h);
static void (*_render)(unsigned int ticks);
static void (*_button)(unsigned int key);
static unsigned int quit;
static unsigned int ticks;

static void setup(void)
{

    view_app_setup();
    view_applist_setup();
    view_front_setup();
    view_install_setup();
    view_repolist_setup();
    view_settings_setup();
    view_sync_setup();
    view_uninstall_setup();

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
    backend_loadbackground("back.png");
    backend_loadfont("habbo.ttf");

}

static void destroy(void)
{

    backend_unloadfont();
    backend_unloadbackground();
    backend_unloadsample("select");
    backend_unloadsample("unselect");
    backend_unloadsample("click");
    backend_destroy();

}

void main_exec(char *name)
{

    char *opkrun = "opkrun";
    char opk[64];
    char *argv[3];

    argv[0] = "opkrun";
    argv[1] = opk;
    argv[2] = 0;

    snprintf(opk, 64, "/media/data/apps/%s", name);
    destroy();
    execvp(opkrun, argv);

}

void main_setview(void (*place)(unsigned int w, unsigned int h), void (*render)(unsigned int ticks), void (*button)(unsigned int key))
{

    _place = place;
    _render = render;
    _button = button;

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
    view_load("front", 0);
    run();
    view_quit("front");
    destroy();

    return 0;

}

