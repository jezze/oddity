#include "include.h"

static struct view view;

static void ondata(unsigned int id, void *data, unsigned int count)
{

    switch (id)
    {

    case 1:
        view_setattr(&view, "text_volume_master", "type", "normal");
        view_setattr(&view, "slider_volume_master", "value", data);

        break;

    case 2:
        view_setattr(&view, "text_volume_pcm", "type", "normal");
        view_setattr(&view, "slider_volume_pcm", "value", data);

        break;

    case 3:
        view_setattr(&view, "text_volume_headphone", "type", "normal");
        view_setattr(&view, "slider_volume_headphone", "value", data);

        break;

    case 4:
        view_setattr(&view, "text_volume_headphones", "type", "normal");
        view_setattr(&view, "slider_volume_headphones", "value", data);

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

static void load(unsigned int type)
{

    view_setattr(&view, "area_volume_master", "selectable", "true");
    view_setattr(&view, "area_volume_pcm", "selectable", "true");
    view_setattr(&view, "area_volume_headphone", "selectable", "true");
    view_setattr(&view, "area_volume_headphones", "selectable", "true");
    view_setattr(&view, "text_volume_master", "type", "disable");
    view_setattr(&view, "text_volume_pcm", "type", "disable");
    view_setattr(&view, "text_volume_headphone", "type", "disable");
    view_setattr(&view, "text_volume_headphones", "type", "disable");
    view_setattr(&view, "slider_volume_master", "value", "-1");
    view_setattr(&view, "slider_volume_pcm", "value", "-1");
    view_setattr(&view, "slider_volume_headphone", "value", "-1");
    view_setattr(&view, "slider_volume_headphones", "value", "-1");
    helper_volume_get(1, "Master", ondata, 0, 0);
    helper_volume_get(2, "PCM", ondata, 0, 0);
    helper_volume_get(3, "Headphone", ondata, 0, 0);
    helper_volume_get(4, "Headphones", ondata, 0, 0);

}

void view_settings_audio_setup(void)
{

    view_init(&view, "settings_audio", load, 0, 0, button);
    widget_area_init(view_createwidget(&view), "area_volume", WIDGET_IN_DEFAULT, 0, 0, 12, 1);
    widget_area_init(view_createwidget(&view), "area_volume_master", WIDGET_IN_DEFAULT, 0, 1, 12, 1);
    widget_area_init(view_createwidget(&view), "area_volume_master_text", WIDGET_IN_DEFAULT, 0, 1, 6, 1);
    widget_area_init(view_createwidget(&view), "area_volume_master_slider", WIDGET_IN_DEFAULT, 6, 1, 6, 1);
    widget_area_init(view_createwidget(&view), "area_volume_pcm", WIDGET_IN_DEFAULT, 0, 2, 12, 1);
    widget_area_init(view_createwidget(&view), "area_volume_pcm_text", WIDGET_IN_DEFAULT, 0, 2, 6, 1);
    widget_area_init(view_createwidget(&view), "area_volume_pcm_slider", WIDGET_IN_DEFAULT, 6, 2, 6, 1);
    widget_area_init(view_createwidget(&view), "area_volume_headphone", WIDGET_IN_DEFAULT, 0, 3, 12, 1);
    widget_area_init(view_createwidget(&view), "area_volume_headphone_text", WIDGET_IN_DEFAULT, 0, 3, 6, 1);
    widget_area_init(view_createwidget(&view), "area_volume_headphone_slider", WIDGET_IN_DEFAULT, 6, 3, 6, 1);
    widget_area_init(view_createwidget(&view), "area_volume_headphones", WIDGET_IN_DEFAULT, 0, 4, 12, 1);
    widget_area_init(view_createwidget(&view), "area_volume_headphones_text", WIDGET_IN_DEFAULT, 0, 4, 6, 1);
    widget_area_init(view_createwidget(&view), "area_volume_headphones_slider", WIDGET_IN_DEFAULT, 6, 4, 6, 1);
    widget_text_init(view_createwidget(&view), WIDGET_ID_DEFAULT, "area_volume", TEXT_TYPE_TITLE, TEXT_ALIGN_CENTER, "Volume");
    widget_text_init(view_createwidget(&view), "text_volume_master", "area_volume_master_text", TEXT_TYPE_DISABLE, TEXT_ALIGN_LEFT, "Master");
    widget_text_init(view_createwidget(&view), "text_volume_pcm", "area_volume_pcm_text", TEXT_TYPE_DISABLE, TEXT_ALIGN_LEFT, "PCM");
    widget_text_init(view_createwidget(&view), "text_volume_headphone", "area_volume_headphone_text", TEXT_TYPE_DISABLE, TEXT_ALIGN_LEFT, "Headphone");
    widget_text_init(view_createwidget(&view), "text_volume_headphones", "area_volume_headphones_text", TEXT_TYPE_DISABLE, TEXT_ALIGN_LEFT, "Headphones");
    widget_slider_init(view_createwidget(&view), "slider_volume_master", "area_volume_master_slider", 0, 100, -1);
    widget_slider_init(view_createwidget(&view), "slider_volume_pcm", "area_volume_pcm_slider", 0, 100, -1);
    widget_slider_init(view_createwidget(&view), "slider_volume_headphone", "area_volume_headphone_slider", 0, 100, -1);
    widget_slider_init(view_createwidget(&view), "slider_volume_headphones", "area_volume_headphones_slider", 0, 100, -1);
    main_registerview(&view);

}

