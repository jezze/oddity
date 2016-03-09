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

int main(int argc, char **argv)
{

    struct view_front *front = view_front_setup(SCREEN_WIDTH, SCREEN_HEIGHT);

    file_init();
    db_init();
    backend_init(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP);
    backend_loadbackground("back.png");
    backend_loadfont("habbo.ttf");
    view_load(&front->base);
    view_loop();
    backend_unloadfont();
    backend_unloadbackground();
    backend_destroy();

    return 0;

}

