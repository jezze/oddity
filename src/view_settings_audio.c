#include <stdlib.h>
#include <stdio.h>
#include "define.h"
#include "box.h"
#include "list.h"
#include "view.h"
#include "main.h"
#include "widget.h"
#include "selection.h"
#include "session.h"

static struct view view;
static struct widget audioarea;
static struct widget audiotext;
static struct widget volumemasterarea;
static struct widget volumemasterareatext;
static struct widget volumemasterareaslider;
static struct widget volumemastertext;
static struct widget volumemasterslider;
static struct widget volumepcmarea;
static struct widget volumepcmareatext;
static struct widget volumepcmareaslider;
static struct widget volumepcmtext;
static struct widget volumepcmslider;
static struct widget volumeheadphonesarea;
static struct widget volumeheadphonesareatext;
static struct widget volumeheadphonesareaslider;
static struct widget volumeheadphonestext;
static struct widget volumeheadphonesslider;
static struct selection selection;

static void place(struct box *size)
{

    widget_area_place(&audioarea, size);
    widget_text_place(&audiotext, &audioarea.size);
    widget_area_place(&volumemasterarea, size);
    widget_area_place(&volumemasterareatext, size);
    widget_area_place(&volumemasterareaslider, size);
    widget_text_place(&volumemastertext, &volumemasterareatext.size);
    widget_slider_place(&volumemasterslider, &volumemasterareaslider.size);
    widget_area_place(&volumepcmarea, size);
    widget_area_place(&volumepcmareatext, size);
    widget_area_place(&volumepcmareaslider, size);
    widget_text_place(&volumepcmtext, &volumepcmareatext.size);
    widget_slider_place(&volumepcmslider, &volumepcmareaslider.size);
    widget_area_place(&volumeheadphonesarea, size);
    widget_area_place(&volumeheadphonesareatext, size);
    widget_area_place(&volumeheadphonesareaslider, size);
    widget_text_place(&volumeheadphonestext, &volumeheadphonesareatext.size);
    widget_slider_place(&volumeheadphonesslider, &volumeheadphonesareaslider.size);

}

static void render(unsigned int ticks)
{

    selection_render(&selection, ticks);
    widget_text_render(&audiotext, ticks);
    widget_text_render(&volumemastertext, ticks);
    widget_slider_render(&volumemasterslider, ticks);
    widget_text_render(&volumepcmtext, ticks);
    widget_slider_render(&volumepcmslider, ticks);
    widget_text_render(&volumeheadphonestext, ticks);
    widget_slider_render(&volumeheadphonesslider, ticks);

}

void ondata(unsigned int id, void *data, unsigned int count)
{

    switch (id)
    {

    case 1:
        volumemastertext.payload.text.color = TEXT_COLOR_NORMAL;
        volumemasterslider.payload.slider.value = strtol(data, 0, 10);

        break;

    case 2:
        volumepcmtext.payload.text.color = TEXT_COLOR_NORMAL;
        volumepcmslider.payload.slider.value = strtol(data, 0, 10);

        break;

    case 3:
        volumeheadphonestext.payload.text.color = TEXT_COLOR_NORMAL;
        volumeheadphonesslider.payload.slider.value = strtol(data, 0, 10);

        break;

    }

}

static void button(unsigned int key)
{

    selection_setclosest(&selection, key);

    switch (key)
    {

    case KEY_LEFT:
        if (selection_isactive(&selection, &volumemasterarea.item))
        {

            session_create("settings_volume_decrement", 1, ondata, 0, 0);
            session_setarg("settings_volume_decrement", 0, "./helper.sh");
            session_setarg("settings_volume_decrement", 1, "volume_decrement");
            session_setarg("settings_volume_decrement", 2, "Master");
            session_setarg("settings_volume_decrement", 3, 0);
            session_run();

        }

        if (selection_isactive(&selection, &volumepcmarea.item))
        {

            session_create("settings_volume_decrement", 2, ondata, 0, 0);
            session_setarg("settings_volume_decrement", 0, "./helper.sh");
            session_setarg("settings_volume_decrement", 1, "volume_decrement");
            session_setarg("settings_volume_decrement", 2, "PCM");
            session_setarg("settings_volume_decrement", 3, 0);
            session_run();

        }

        if (selection_isactive(&selection, &volumeheadphonesarea.item))
        {

            session_create("settings_volume_decrement", 3, ondata, 0, 0);
            session_setarg("settings_volume_decrement", 0, "./helper.sh");
            session_setarg("settings_volume_decrement", 1, "volume_decrement");
            session_setarg("settings_volume_decrement", 2, "Headphones");
            session_setarg("settings_volume_decrement", 3, 0);
            session_run();

        }

        break;

    case KEY_RIGHT:
        if (selection_isactive(&selection, &volumemasterarea.item))
        {

            session_create("settings_volume_increment", 1, ondata, 0, 0);
            session_setarg("settings_volume_increment", 0, "./helper.sh");
            session_setarg("settings_volume_increment", 1, "volume_increment");
            session_setarg("settings_volume_increment", 2, "Master");
            session_setarg("settings_volume_increment", 3, 0);
            session_run();

        }

        if (selection_isactive(&selection, &volumepcmarea.item))
        {

            session_create("settings_volume_increment", 2, ondata, 0, 0);
            session_setarg("settings_volume_increment", 0, "./helper.sh");
            session_setarg("settings_volume_increment", 1, "volume_increment");
            session_setarg("settings_volume_increment", 2, "PCM");
            session_setarg("settings_volume_increment", 3, 0);
            session_run();

        }

        if (selection_isactive(&selection, &volumeheadphonesarea.item))
        {

            session_create("settings_volume_increment", 3, ondata, 0, 0);
            session_setarg("settings_volume_increment", 0, "./helper.sh");
            session_setarg("settings_volume_increment", 1, "volume_increment");
            session_setarg("settings_volume_increment", 2, "Headphones");
            session_setarg("settings_volume_increment", 3, 0);
            session_run();

        }

        break;

    }

    selection_unselect(&selection, key, "settings_audio");

}

static void load(void)
{

    volumemastertext.payload.text.color = TEXT_COLOR_DISABLE;
    volumemasterslider.payload.slider.value = -1;
    volumepcmtext.payload.text.color = TEXT_COLOR_DISABLE;
    volumepcmslider.payload.slider.value = -1;
    volumeheadphonestext.payload.text.color = TEXT_COLOR_DISABLE;
    volumeheadphonesslider.payload.slider.value = -1;

    session_create("settings_volume_get_master", 1, ondata, 0, 0);
    session_setarg("settings_volume_get_master", 0, "./helper.sh");
    session_setarg("settings_volume_get_master", 1, "volume_get");
    session_setarg("settings_volume_get_master", 2, "Master");
    session_setarg("settings_volume_get_master", 3, 0);
    session_create("settings_volume_get_pcm", 2, ondata, 0, 0);
    session_setarg("settings_volume_get_pcm", 0, "./helper.sh");
    session_setarg("settings_volume_get_pcm", 1, "volume_get");
    session_setarg("settings_volume_get_pcm", 2, "PCM");
    session_setarg("settings_volume_get_pcm", 3, 0);
    session_create("settings_volume_get_headphones", 3, ondata, 0, 0);
    session_setarg("settings_volume_get_headphones", 0, "./helper.sh");
    session_setarg("settings_volume_get_headphones", 1, "volume_get");
    session_setarg("settings_volume_get_headphones", 2, "Headphones");
    session_setarg("settings_volume_get_headphones", 3, 0);
    session_run();
    main_setview(place, render, button);
    selection_reset(&selection);

}

void view_settings_audio_setup(void)
{

    view_init(&view, "settings_audio", load, 0);
    view_register(&view);
    widget_area_init(&audioarea, 0, 0, 8, 1);
    widget_text_init(&audiotext, TEXT_COLOR_TITLE, TEXT_ALIGN_CENTER, "Volume");
    widget_area_init(&volumemasterarea, 0, 1, 8, 1);
    widget_area_init(&volumemasterareatext, 0, 1, 5, 1);
    widget_area_init(&volumemasterareaslider, 5, 1, 3, 1);
    widget_text_init(&volumemastertext, TEXT_COLOR_DISABLE, TEXT_ALIGN_LEFT, "Master");
    widget_slider_init(&volumemasterslider, 0, 100, -1);
    selection_add(&selection, &volumemasterarea.item);
    widget_area_init(&volumepcmarea, 0, 2, 8, 1);
    widget_area_init(&volumepcmareatext, 0, 2, 5, 1);
    widget_area_init(&volumepcmareaslider, 5, 2, 3, 1);
    widget_text_init(&volumepcmtext, TEXT_COLOR_DISABLE, TEXT_ALIGN_LEFT, "PCM");
    widget_slider_init(&volumepcmslider, 0, 100, -1);
    selection_add(&selection, &volumepcmarea.item);
    widget_area_init(&volumeheadphonesarea, 0, 3, 8, 1);
    widget_area_init(&volumeheadphonesareatext, 0, 3, 5, 1);
    widget_area_init(&volumeheadphonesareaslider, 5, 3, 3, 1);
    widget_text_init(&volumeheadphonestext, TEXT_COLOR_DISABLE, TEXT_ALIGN_LEFT, "Headphones");
    widget_slider_init(&volumeheadphonesslider, 0, 100, -1);
    selection_add(&selection, &volumeheadphonesarea.item);

}

