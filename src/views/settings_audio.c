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

static void onbutton(unsigned int key)
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

    view_unselect(&view, key);

}

static void onload(unsigned int type)
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

    view_init(&view, "settings_audio", onload, 0, 0, onbutton);
    pool_create_area(&view, "area_volume_master", WIDGET_IN_DEFAULT, 1, 1, 10, 1);
    pool_create_area(&view, "area_volume_master_text", WIDGET_IN_DEFAULT, 1, 1, 5, 1);
    pool_create_area(&view, "area_volume_master_slider", WIDGET_IN_DEFAULT, 6, 1, 5, 1);
    pool_create_area(&view, "area_volume_pcm", WIDGET_IN_DEFAULT, 1, 2, 10, 1);
    pool_create_area(&view, "area_volume_pcm_text", WIDGET_IN_DEFAULT, 1, 2, 5, 1);
    pool_create_area(&view, "area_volume_pcm_slider", WIDGET_IN_DEFAULT, 6, 2, 5, 1);
    pool_create_area(&view, "area_volume_headphone", WIDGET_IN_DEFAULT, 1, 3, 10, 1);
    pool_create_area(&view, "area_volume_headphone_text", WIDGET_IN_DEFAULT, 1, 3, 5, 1);
    pool_create_area(&view, "area_volume_headphone_slider", WIDGET_IN_DEFAULT, 6, 3, 5, 1);
    pool_create_area(&view, "area_volume_headphones", WIDGET_IN_DEFAULT, 1, 4, 10, 1);
    pool_create_area(&view, "area_volume_headphones_text", WIDGET_IN_DEFAULT, 1, 4, 5, 1);
    pool_create_area(&view, "area_volume_headphones_slider", WIDGET_IN_DEFAULT, 6, 4, 5, 1);
    pool_create_text(&view, "text_volume_master", "area_volume_master_text", TEXT_TYPE_DISABLE, TEXT_ALIGN_LEFT, "Master");
    pool_create_text(&view, "text_volume_pcm", "area_volume_pcm_text", TEXT_TYPE_DISABLE, TEXT_ALIGN_LEFT, "PCM");
    pool_create_text(&view, "text_volume_headphone", "area_volume_headphone_text", TEXT_TYPE_DISABLE, TEXT_ALIGN_LEFT, "Headphone");
    pool_create_text(&view, "text_volume_headphones", "area_volume_headphones_text", TEXT_TYPE_DISABLE, TEXT_ALIGN_LEFT, "Headphones");
    pool_create_slider(&view, "slider_volume_master", "area_volume_master_slider", 0, 100, -1);
    pool_create_slider(&view, "slider_volume_pcm", "area_volume_pcm_slider", 0, 100, -1);
    pool_create_slider(&view, "slider_volume_headphone", "area_volume_headphone_slider", 0, 100, -1);
    pool_create_slider(&view, "slider_volume_headphones", "area_volume_headphones_slider", 0, 100, -1);
    main_registerview(&view);

}

