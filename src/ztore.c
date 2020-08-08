#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "box.h"
#include "text.h"
#include "menu.h"
#include "file.h"
#include "db.h"
#include "view.h"
#include "view_front.h"
#include "backend.h"
#include "ztore.h"

#define SCREEN_WIDTH                    320
#define SCREEN_HEIGHT                   240
#define SCREEN_BPP                      32

static unsigned int quit;

static void (*_place)(unsigned int w, unsigned int h);
static void (*_render)(void);
static void (*_button)(unsigned int key);

void ztore_quit(void)
{

    quit = 1;

}

void ztore_setview(void (*place)(unsigned int w, unsigned int h), void (*render)(void), void (*button)(unsigned int key))
{

    _place = place;
    _render = render;
    _button = button;

}

static void ztore_loop(void)
{

    while (!quit)
    {

        backend_pollevent(ztore_quit, _button);
        _place(SCREEN_WIDTH, SCREEN_HEIGHT);
        backend_render(_render);
        backend_sleep(1000 / 60);

    }

}

static void ztore_init(void)
{

    backend_init(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP);
    backend_loadbackground("back.png");
    backend_loadfont("habbo.ttf");

}

static void ztore_destroy(void)
{

    backend_unloadfont();
    backend_unloadbackground();
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

    struct view *front = view_front_setup();

    file_init();
    db_init();
    ztore_init();
    view_load(front, 0);
    ztore_loop();
    view_quit(front);
    ztore_destroy();

    return 0;

}

