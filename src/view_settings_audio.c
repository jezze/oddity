#include <stdlib.h>
#include <stdio.h>
#include "define.h"
#include "box.h"
#include "list.h"
#include "widget.h"
#include "view.h"
#include "session.h"
#include "helper.h"
#include "main.h"

static struct view view;

void ondata(unsigned int id, void *data, unsigned int count)
{

    switch (id)
    {

    case 1:
        view_findwidget(&view, "text_volume_master")->payload.text.color = TEXT_COLOR_NORMAL;
        view_findwidget(&view, "slider_volume_master")->payload.slider.value = strtol(data, 0, 10);

        break;

    case 2:
        view_findwidget(&view, "text_volume_pcm")->payload.text.color = TEXT_COLOR_NORMAL;
        view_findwidget(&view, "slider_volume_pcm")->payload.slider.value = strtol(data, 0, 10);

        break;

    case 3:
        view_findwidget(&view, "text_volume_headphone")->payload.text.color = TEXT_COLOR_NORMAL;
        view_findwidget(&view, "slider_volume_headphone")->payload.slider.value = strtol(data, 0, 10);

        break;

    case 4:
        view_findwidget(&view, "text_volume_headphones")->payload.text.color = TEXT_COLOR_NORMAL;
        view_findwidget(&view, "slider_volume_headphones")->payload.slider.value = strtol(data, 0, 10);

        break;

    }

}

static void button(unsigned int key)
{

    view_moveselection(&view, key);

    switch (key)
    {

    case KEY_LEFT:
        if (view_isactive(&view, "area_volume_master"))
            helper_volume_decrement(1, "Master", ondata, 0, 0);

        if (view_isactive(&view, "area_volume_pcm"))
            helper_volume_decrement(2, "PCM", ondata, 0, 0);

        if (view_isactive(&view, "area_volume_headphone"))
            helper_volume_decrement(3, "Headphone", ondata, 0, 0);

        if (view_isactive(&view, "area_volume_headphones"))
            helper_volume_decrement(4, "Headphones", ondata, 0, 0);

        break;

    case KEY_RIGHT:
        if (view_isactive(&view, "area_volume_master"))
            helper_volume_increment(1, "Master", ondata, 0, 0);

        if (view_isactive(&view, "area_volume_pcm"))
            helper_volume_increment(2, "PCM", ondata, 0, 0);

        if (view_isactive(&view, "area_volume_headphone"))
            helper_volume_increment(3, "Headphone", ondata, 0, 0);

        if (view_isactive(&view, "area_volume_headphones"))
            helper_volume_increment(4, "Headphones", ondata, 0, 0);

        break;

    }

    view_unselect(&view, key, view.name);

}

static void load(void)
{

    view_findwidget(&view, "text_volume_master")->payload.text.color = TEXT_COLOR_DISABLE;
    view_findwidget(&view, "slider_volume_master")->payload.slider.value = -1;
    view_findwidget(&view, "text_volume_pcm")->payload.text.color = TEXT_COLOR_DISABLE;
    view_findwidget(&view, "slider_volume_pcm")->payload.slider.value = -1;
    view_findwidget(&view, "text_volume_headphone")->payload.text.color = TEXT_COLOR_DISABLE;
    view_findwidget(&view, "slider_volume_headphone")->payload.slider.value = -1;
    view_findwidget(&view, "text_volume_headphones")->payload.text.color = TEXT_COLOR_DISABLE;
    view_findwidget(&view, "slider_volume_headphones")->payload.slider.value = -1;
    view_findwidget(&view, "area_volume_master")->selectable = 1;
    view_findwidget(&view, "area_volume_pcm")->selectable = 1;
    view_findwidget(&view, "area_volume_headphone")->selectable = 1;
    view_findwidget(&view, "area_volume_headphones")->selectable = 1;

    helper_volume_get(1, "Master", ondata, 0, 0);
    helper_volume_get(2, "PCM", ondata, 0, 0);
    helper_volume_get(3, "Headphone", ondata, 0, 0);
    helper_volume_get(4, "Headphones", ondata, 0, 0);
    view_reset(&view);

}

void view_settings_audio_setup(void)
{

    widget_area_init(view_createwidget(&view), "area_volume", WIDGET_IN_DEFAULT, 0, 0, 12, 1);
    widget_text_init(view_createwidget(&view), WIDGET_ID_DEFAULT, "area_volume", TEXT_COLOR_TITLE, TEXT_ALIGN_CENTER, "Volume");
    widget_area_init(view_createwidget(&view), "area_volume_master", WIDGET_IN_DEFAULT, 0, 1, 12, 1);
    widget_area_init(view_createwidget(&view), "area_volume_master_text", WIDGET_IN_DEFAULT, 0, 1, 8, 1);
    widget_area_init(view_createwidget(&view), "area_volume_master_slider", WIDGET_IN_DEFAULT, 8, 1, 4, 1);
    widget_text_init(view_createwidget(&view), "text_volume_master", "area_volume_master_text", TEXT_COLOR_DISABLE, TEXT_ALIGN_LEFT, "Master");
    widget_slider_init(view_createwidget(&view), "slider_volume_master", "area_volume_master_slider", 0, 100, -1);
    widget_area_init(view_createwidget(&view), "area_volume_pcm", WIDGET_IN_DEFAULT, 0, 2, 12, 1);
    widget_area_init(view_createwidget(&view), "area_volume_pcm_text", WIDGET_IN_DEFAULT, 0, 2, 8, 1);
    widget_area_init(view_createwidget(&view), "area_volume_pcm_slider", WIDGET_IN_DEFAULT, 8, 2, 4, 1);
    widget_text_init(view_createwidget(&view), "text_volume_pcm", "area_volume_pcm_text", TEXT_COLOR_DISABLE, TEXT_ALIGN_LEFT, "PCM");
    widget_slider_init(view_createwidget(&view), "slider_volume_pcm", "area_volume_pcm_slider", 0, 100, -1);
    widget_area_init(view_createwidget(&view), "area_volume_headphone", WIDGET_IN_DEFAULT, 0, 3, 12, 1);
    widget_area_init(view_createwidget(&view), "area_volume_headphone_text", WIDGET_IN_DEFAULT, 0, 3, 8, 1);
    widget_area_init(view_createwidget(&view), "area_volume_headphone_slider", WIDGET_IN_DEFAULT, 8, 3, 4, 1);
    widget_text_init(view_createwidget(&view), "text_volume_headphone", "area_volume_headphone_text", TEXT_COLOR_DISABLE, TEXT_ALIGN_LEFT, "Headphone");
    widget_slider_init(view_createwidget(&view), "slider_volume_headphone", "area_volume_headphone_slider", 0, 100, -1);
    widget_area_init(view_createwidget(&view), "area_volume_headphones", WIDGET_IN_DEFAULT, 0, 4, 12, 1);
    widget_area_init(view_createwidget(&view), "area_volume_headphones_text", WIDGET_IN_DEFAULT, 0, 4, 8, 1);
    widget_area_init(view_createwidget(&view), "area_volume_headphones_slider", WIDGET_IN_DEFAULT, 8, 4, 4, 1);
    widget_text_init(view_createwidget(&view), "text_volume_headphones", "area_volume_headphones_text", TEXT_COLOR_DISABLE, TEXT_ALIGN_LEFT, "Headphones");
    widget_slider_init(view_createwidget(&view), "slider_volume_headphones", "area_volume_headphones_slider", 0, 100, -1);
    view_init(&view, "settings_audio", load, 0, 0, button);
    main_registerview(&view);

}

