#include <stdlib.h>
#include "define.h"
#include "box.h"
#include "list.h"
#include "widget.h"
#include "view.h"
#include "main.h"

static struct view view;
static struct widget autostartarea;
static struct widget autostarttext;

static void button(unsigned int key)
{

    view_moveselection(&view, key);
    view_unselect(&view, key, view.name);

}

static void load(void)
{

    view_reset(&view);

}

void view_settings_system_setup(void)
{

    widget_area_init(&autostartarea, "area_autostart", WIDGET_IN_DEFAULT, 0, 0, 8, 1);
    widget_text_init(&autostarttext, WIDGET_ID_DEFAULT, "area_autostart", TEXT_COLOR_NORMAL, TEXT_ALIGN_LEFT, "Autostart");
    view_addselection(&view, &autostartarea);
    view_init(&view, "settings_system", load, 0, 0, button);
    view_register(&view, &autostartarea);
    view_register(&view, &autostarttext);
    main_registerview(&view);

}

