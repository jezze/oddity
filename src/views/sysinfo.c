#include "include.h"

static struct view view;
static char text[512];
static unsigned int offset;

static void ondata(unsigned int id, void *data, unsigned int count)
{

    memcpy(text + offset, data, count);

    offset += count;

}

static void onbutton(unsigned int key)
{

    view_moveselection(&view, key);
    view_unselect(&view, key);

}

static void onload(unsigned int type)
{

    helper_sysinfo_get(1, ondata, 0, 0);

}

void view_sysinfo_setup(void)
{

    view_init(&view, "sysinfo", onload, 0, 0, onbutton);
    pool_create_area(&view, "area_sysinfo", WIDGET_IN_DEFAULT, 0, 0, 12, 6);
    pool_create_text(&view, WIDGET_ID_DEFAULT, "area_sysinfo", TEXT_TYPE_NORMAL, TEXT_ALIGN_LEFT, text);
    main_registerview(&view);

}

