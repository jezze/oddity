#include <stdlib.h>
#include "box.h"
#include "text.h"
#include "menu.h"
#include "file.h"
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

void ztore_flipview(struct view *view)
{

    currentview = view;

}

int main(int argc, char **argv)
{

    struct view_front *front;

    file_init();
    backend_init(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP);
    backend_loadbackground("back.png");
    backend_loadfont("habbo.ttf");

    front = view_front_setup(SCREEN_WIDTH, SCREEN_HEIGHT);
    currentview = &front->base;

    backend_prerender();
    currentview->render();
    backend_postrender();

    while (!quit)
    {

        backend_waitevent(currentview);
        backend_prerender();
        currentview->render();
        backend_postrender();

    }

    backend_unloadbackground();
    backend_unloadfont();
    backend_destroy();

    return 0;

}

