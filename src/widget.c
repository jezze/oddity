#include <stdlib.h>
#include <string.h>
#include "config.h"
#include "box.h"
#include "list.h"
#include "text.h"
#include "widget.h"
#include "view.h"
#include "backend.h"

#define DIVX 12
#define DIVY 8
#define BORX 1
#define BORY 2
#define PADX 8
#define PADY 6

struct attribute
{

    const char *name;
    unsigned int value;

};

static unsigned int findattribute(const struct attribute attributes[], unsigned int count, char *name)
{

    unsigned int i;

    for (i = 0; i < count; i++)
    {

        const struct attribute *attribute = &attributes[i];

        if (!strcmp(attribute->name, name))
            return attribute->value;

    }

    return 0;

}

void widget_area_place(struct widget *widget, const struct box *box)
{

    struct widget_area *area = &widget->payload.area;
    unsigned int x = box->w / DIVX - BORX;
    unsigned int y = box->h / DIVY - BORY;
    unsigned int mx = (box->w - (x * DIVX)) / 2;
    unsigned int my = (box->h - (y * DIVY)) / 2;

    widget->size.x = x * area->x + mx;
    widget->size.y = y * area->y + my;
    widget->size.w = x * area->w;
    widget->size.h = y * area->h;

}

void widget_area_render(const struct widget *widget, unsigned int ticks)
{

}

void widget_area_set(struct widget *widget, const char *key, char *value)
{

}

void widget_area_init(struct widget *widget, char *id, char *in, int x, int y, int w, int h)
{

    struct widget_area *area = &widget->payload.area;

    widget_init(widget, WIDGET_TYPE_AREA, id, in);

    area->x = x;
    area->y = y;
    area->w = w;
    area->h = h;

}

void widget_icon_place(struct widget *widget, const struct box *box)
{

    widget->size.x = box->x;
    widget->size.y = box->y;
    widget->size.w = box->w;
    widget->size.h = box->h;

}

void widget_icon_render(const struct widget *widget, unsigned int ticks)
{

    const struct widget_icon *icon = &widget->payload.icon;
    struct box box;

    box.x = widget->size.x + widget->size.w / 2 - 20;
    box.y = widget->size.y + PADY + 8;
    box.w = widget->size.w - PADX * 2;
    box.h = widget->size.h - PADY * 2;

    backend_paint_glyph("bigicon", icon->type, box.x, box.y, box.w, box.h, COLOR_TITLE);

}

void widget_icon_set(struct widget *widget, const char *key, char *value)
{

}

void widget_icon_init(struct widget *widget, char *id, char *in, unsigned int type)
{

    struct widget_icon *icon = &widget->payload.icon;

    widget_init(widget, WIDGET_TYPE_ICON, id, in);

    icon->type = type;

}

void widget_option_place(struct widget *widget, const struct box *box)
{

    widget->size.x = box->x;
    widget->size.y = box->y;
    widget->size.w = box->w;
    widget->size.h = box->h;

}

void widget_option_render(const struct widget *widget, unsigned int ticks)
{

    const struct widget_option *option = &widget->payload.option;
    struct box box;

    box.x = widget->size.x + PADX;
    box.y = widget->size.y + PADY;
    box.w = widget->size.w - PADX * 2;
    box.h = widget->size.h - PADY * 2;

    if (option->value)
        text_render("default", &box, TEXT_TYPE_TITLE, TEXT_ALIGN_CENTER, option->value);

}

void widget_option_set(struct widget *widget, const char *key, char *value)
{

    struct widget_option *option = &widget->payload.option;

    if (!strcmp(key, "name"))
        option->name = value;

    if (!strcmp(key, "value"))
        option->value = value;

}

void widget_option_init(struct widget *widget, char *id, char *in, char *name, char *value)
{

    struct widget_option *option = &widget->payload.option;

    widget_init(widget, WIDGET_TYPE_OPTION, id, in);

    widget->hidden = 1;
    option->name = name;
    option->value = value;

}

void widget_select_place(struct widget *widget, const struct box *box)
{

    widget->size.x = box->x;
    widget->size.y = box->y;
    widget->size.w = box->w;
    widget->size.h = box->h;

}

void widget_select_render(const struct widget *widget, unsigned int ticks)
{

    struct box box;

    box.x = widget->size.x + PADX;
    box.y = widget->size.y + PADY;
    box.w = widget->size.w - PADX * 2;
    box.h = widget->size.h - PADY * 2;

    text_render("default", &box, TEXT_TYPE_SELECT, TEXT_ALIGN_LEFT, "<");
    text_render("default", &box, TEXT_TYPE_SELECT, TEXT_ALIGN_RIGHT, ">");

}

void widget_select_set(struct widget *widget, const char *key, char *value)
{

    struct widget_select *select = &widget->payload.select;

    if (!strcmp(key, "value"))
        select->value = value;

}

void widget_select_init(struct widget *widget, char *id, char *in, char *value)
{

    struct widget_select *select = &widget->payload.select;

    widget_init(widget, WIDGET_TYPE_SELECT, id, in);

    select->value = value;

}

void widget_slider_place(struct widget *widget, const struct box *box)
{

    widget->size.x = box->x;
    widget->size.y = box->y;
    widget->size.w = box->w;
    widget->size.h = box->h;

}

void widget_slider_render(const struct widget *widget, unsigned int ticks)
{

    const struct widget_slider *slider = &widget->payload.slider;
    struct box box;

    box.x = widget->size.x + PADX;
    box.y = widget->size.y + PADY;
    box.w = widget->size.w - PADX * 2;
    box.h = widget->size.h - PADY * 2;

    text_render("default", &box, TEXT_TYPE_SELECT, TEXT_ALIGN_LEFT, "<");
    text_render("default", &box, TEXT_TYPE_SELECT, TEXT_ALIGN_RIGHT, ">");

    box.x += 16;
    box.w -= 32;

    if (slider->value >= 0)
    {

        unsigned int ws = box.w * ((float)slider->value / (float)100);

        backend_paint_slider(box.x, box.y, box.w, box.h, ws, ticks);

    }

    else
    {

        backend_paint_slider(box.x, box.y, box.w, box.h, -1, ticks);

    }

}

void widget_slider_set(struct widget *widget, const char *key, char *value)
{

    struct widget_slider *slider = &widget->payload.slider;

    if (!strcmp(key, "value"))
        slider->value = strtol(value, 0, 10);

}

void widget_slider_init(struct widget *widget, char *id, char *in, int min, int max, int value)
{

    struct widget_slider *slider = &widget->payload.slider;

    widget_init(widget, WIDGET_TYPE_SLIDER, id, in);

    slider->min = min;
    slider->max = max;
    slider->value = value;

}

void widget_text_place(struct widget *widget, const struct box *box)
{

    widget->size.x = box->x;
    widget->size.y = box->y;
    widget->size.w = box->w;
    widget->size.h = box->h;

}

void widget_text_render(const struct widget *widget, unsigned int ticks)
{

    const struct widget_text *text = &widget->payload.text;
    struct box box;

    box.x = widget->size.x + PADX;
    box.y = widget->size.y + PADY;
    box.w = widget->size.w - PADX * 2;
    box.h = widget->size.h - PADY * 2;

    if (text->data)
        text_render("default", &box, text->type, text->align, text->data);

}

static const struct attribute widget_text_type[] = {
    {"normal", TEXT_TYPE_NORMAL},
    {"title", TEXT_TYPE_TITLE},
    {"select", TEXT_TYPE_SELECT},
    {"disable", TEXT_TYPE_DISABLE}
};

static const struct attribute widget_text_align[] = {
    {"left", TEXT_ALIGN_LEFT},
    {"right", TEXT_ALIGN_RIGHT},
    {"center", TEXT_ALIGN_CENTER}
};

void widget_text_set(struct widget *widget, const char *key, char *value)
{

    struct widget_text *text = &widget->payload.text;

    if (!strcmp(key, "type"))
        text->type = findattribute(widget_text_type, 4, value);

    if (!strcmp(key, "align"))
        text->align = findattribute(widget_text_align, 3, value);

    if (!strcmp(key, "data"))
        text->data = value;

}

void widget_text_init(struct widget *widget, char *id, char *in, unsigned int type, unsigned int align, char *data)
{

    struct widget_text *text = &widget->payload.text;

    widget_init(widget, WIDGET_TYPE_TEXT, id, in);

    text->type = type;
    text->align = align;
    text->data = data;

}

void widget_toggle_place(struct widget *widget, const struct box *box)
{

    widget->size.x = box->x;
    widget->size.y = box->y;
    widget->size.w = box->w;
    widget->size.h = box->h;

}

void widget_toggle_render(const struct widget *widget, unsigned int ticks)
{

    const struct widget_toggle *toggle = &widget->payload.toggle;
    struct box box;

    box.x = widget->size.x + PADX;
    box.y = widget->size.y + PADY;
    box.w = widget->size.w - PADX * 2;
    box.h = widget->size.h - PADY * 2;

    switch (toggle->state)
    {

    case TOGGLE_OFF:
        text_render("default", &box, TEXT_TYPE_NORMAL, TEXT_ALIGN_RIGHT, "Off");

        break;
    case TOGGLE_ON:
        text_render("default", &box, TEXT_TYPE_NORMAL, TEXT_ALIGN_RIGHT, "On");

        break;

    case TOGGLE_OFF_DISABLED:
        text_render("default", &box, TEXT_TYPE_DISABLE, TEXT_ALIGN_RIGHT, "Off");

        break;

    case TOGGLE_ON_DISABLED:
        text_render("default", &box, TEXT_TYPE_DISABLE, TEXT_ALIGN_RIGHT, "On");

        break;

    }

}

static const struct attribute widget_toggle_state[] = {
    {"off", TOGGLE_OFF},
    {"on", TOGGLE_ON},
    {"off_disabled", TOGGLE_OFF_DISABLED},
    {"on_disabled", TOGGLE_ON_DISABLED}
};

void widget_toggle_set(struct widget *widget, const char *key, char *value)
{

    struct widget_toggle *toggle = &widget->payload.toggle;

    if (!strcmp(key, "state"))
        toggle->state = findattribute(widget_toggle_state, 4, value);

}

void widget_toggle_init(struct widget *widget, char *id, char *in, unsigned int state)
{

    struct widget_toggle *toggle = &widget->payload.toggle;

    widget_init(widget, WIDGET_TYPE_TOGGLE, id, in);

    toggle->state = state;

}

void widget_init(struct widget *widget, unsigned int type, char *id, char *in)
{

    widget->type = type;
    widget->id = id;
    widget->in = in;
    widget->hidden = 0;
    widget->selectable = 0;

    list_inititem(&widget->item, widget);

}

