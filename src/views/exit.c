#include "include.h"

static struct view view;

static void load(void)
{

    main_quit();

}

void view_exit_setup(void)
{

    view_init(&view, "exit", load, 0, 0, 0);
    main_registerview(&view);

}

