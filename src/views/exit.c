#include "include.h"

static struct view view;

static void onload(unsigned int type)
{

    main_quit();

}

void view_exit_setup(void)
{

    view_init(&view, "exit", onload, 0, 0, 0);
    main_registerview(&view);

}

