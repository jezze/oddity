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

static struct view *currentview;
static unsigned int quit;

void ztore_quit()
{

    quit = 1;

}

void ztore_redraw()
{

    backend_pollevent(ztore_quit, currentview->keydown);
    backend_render(currentview->render);

}

void ztore_exec(char *name)
{

    char command[64];

    snprintf(command, 64, "opkrun /media/data/apps/%s", name);
    backend_unloadfont();
    backend_unloadbackground();
    backend_destroy();
    system(command);
    backend_init(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP);
    backend_loadbackground("back.png");
    backend_loadfont("habbo.ttf");

}

void ztore_load(struct view *view)
{

    currentview = view;

    currentview->load();

}

int main(int argc, char **argv)
{

    struct view_front *front;

    file_init();
    db_init();
    backend_init(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP);
    backend_loadbackground("back.png");
    backend_loadfont("habbo.ttf");

    front = view_front_setup(SCREEN_WIDTH, SCREEN_HEIGHT);

    ztore_load(&front->base);
    backend_render(currentview->render);

    while (!quit)
    {

        backend_waitevent(ztore_quit, currentview->keydown);
        backend_render(currentview->render);

    }

    backend_unloadfont();
    backend_unloadbackground();
    backend_destroy();

    return 0;

}

