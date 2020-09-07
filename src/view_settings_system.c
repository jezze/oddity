#include <stdlib.h>
#include "define.h"
#include "box.h"
#include "list.h"
#include "widget.h"
#include "view.h"
#include "main.h"

static struct view view;

static void button(unsigned int key)
{

    view_moveselection(&view, key);

    switch (key)
    {

    case KEY_A:
        if (view_isactive(&view, "area_autostart"))
        {

            struct widget *widget = view_findwidget(&view, "toggle_autostart");

            widget->payload.toggle.on = !widget->payload.toggle.on;

        }

        break;

    }

    view_unselect(&view, key, view.name);

}

static void load(void)
{

    view_reset(&view);

}

void view_settings_system_setup(void)
{

    widget_area_init(view_createwidget(&view), "area_autostart", WIDGET_IN_DEFAULT, 0, 0, 8, 1);
    widget_text_init(view_createwidget(&view), WIDGET_ID_DEFAULT, "area_autostart", TEXT_COLOR_NORMAL, TEXT_ALIGN_LEFT, "Autostart");
    widget_toggle_init(view_createwidget(&view), "toggle_autostart", "area_autostart", 0);
    view_addselection(&view, "area_autostart");
    view_init(&view, "settings_system", load, 0, 0, button);
    main_registerview(&view);

}

