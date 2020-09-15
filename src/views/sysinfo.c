#include "include.h"

static struct view view;
static char text[512];
static unsigned int offset;

static void ondata(unsigned int id, void *data, unsigned int count)
{

    memcpy(text + offset, data, count);

}

static void button(unsigned int key)
{

    view_moveselection(&view, key);
    view_unselect(&view, key, view.name);

}

static void load(void)
{

    view_reset(&view);
    helper_sysinfo_get(1, ondata, 0, 0);

}

void view_sysinfo_setup(void)
{

    view_init(&view, "sysinfo", load, 0, 0, button);
    widget_area_init(view_createwidget(&view), "area_sysinfo", WIDGET_IN_DEFAULT, 0, 0, 12, 6);
    widget_text_init(view_createwidget(&view), WIDGET_ID_DEFAULT, "area_sysinfo", TEXT_TYPE_NORMAL, TEXT_ALIGN_LEFT, text);
    main_registerview(&view);

}

