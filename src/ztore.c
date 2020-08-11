#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "box.h"
#include "text.h"
#include "menu.h"
#include "session.h"
#include "file.h"
#include "db.h"
#include "view.h"
#include "backend.h"
#include "ztore.h"

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

void ztore_quit(void)
{

    quit = 1;

}

void ztore_setview(void (*place)(unsigned int w, unsigned int h), void (*render)(unsigned int ticks), void (*button)(unsigned int key))
{

    _place = place;
    _render = render;
    _button = button;

}

static unsigned int ticks;

static void ztore_loop(void)
{

    unsigned int limit = 1000 / 60;
    unsigned int frametime;

    while (!quit)
    {

        frametime = backend_ticks();

        session_poll();
        backend_pollevent(ztore_quit, _button);
        backend_render(ticks, _place, _render);

        if (backend_ticks() - frametime < limit)
            backend_delay(limit - (backend_ticks() - frametime));

        ticks++;

    }

}

static void ztore_init(void)
{

    backend_init();
    backend_loadaudio();
    backend_loadbackground("back.png");
    backend_loadfont("habbo.ttf");

}

static void ztore_destroy(void)
{

    backend_unloadfont();
    backend_unloadbackground();
    backend_unloadaudio();
    backend_destroy();

}

void ztore_exec(char *name)
{

    char *opkrun = "opkrun";
    char opk[64];
    char *argv[3];

    argv[0] = "opkrun";
    argv[1] = opk;
    argv[2] = 0;

    snprintf(opk, 64, "/media/data/apps/%s", name);
    ztore_destroy();
    execvp(opkrun, argv);

}

int main(int argc, char **argv)
{

    file_init();
    db_init();
    ztore_init();
    view_app_setup();
    view_applist_setup();
    view_front_setup();
    view_install_setup();
    view_repolist_setup();
    view_settings_setup();
    view_sync_setup();
    view_uninstall_setup();
    view_load("front", 0);
    ztore_loop();
    view_quit("front");
    ztore_destroy();

    return 0;

}

