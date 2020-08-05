#include <stdlib.h>
#include <stdio.h>
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

static void (*_render)(void);
static void (*_keydown)(unsigned int key);

void ztore_quit(void)
{

    quit = 1;

}

void ztore_setview(void (*render)(void), void (*keydown)(unsigned int key))
{

    _render = render;
    _keydown = keydown;

}

static void ztore_loop(void)
{

    while (!quit)
    {

        backend_render(_render);
        backend_waitevent(ztore_quit, _keydown);

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

    char command[64];

    snprintf(command, 64, "opkrun /media/data/apps/%s", name);
    ztore_destroy();
    system(command);
    ztore_init();

}

void ztore_redraw(void)
{

    backend_pollevent(ztore_quit, _keydown);
    backend_render(_render);

}

int main(int argc, char **argv)
{

    struct view *front = view_front_setup(SCREEN_WIDTH, SCREEN_HEIGHT);

    file_init();
    db_init();
    ztore_init();
    view_load(front, 0);
    ztore_loop();
    view_quit(front);
    ztore_destroy();

    return 0;

}

