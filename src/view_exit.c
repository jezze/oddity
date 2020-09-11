#include <stdlib.h>
#include "define.h"
#include "box.h"
#include "list.h"
#include "widget.h"
#include "view.h"
#include "main.h"

static struct view exitview;

static void load(void)
{

    main_quit();

}

void view_exit_setup(void)
{

    view_init(&exitview, "exit", load, 0, 0, 0);
    main_registerview(&exitview);

}

