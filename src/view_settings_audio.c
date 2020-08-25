#include <stdlib.h>
#include <stdio.h>
#include "define.h"
#include "box.h"
#include "list.h"
#include "widget.h"
#include "selection.h"
#include "view.h"
#include "session.h"
#include "main.h"

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
static struct widget volumeheadphonearea;
static struct widget volumeheadphoneareatext;
static struct widget volumeheadphoneareaslider;
static struct widget volumeheadphonetext;
static struct widget volumeheadphoneslider;
static struct widget volumeheadphonesarea;
static struct widget volumeheadphonesareatext;
static struct widget volumeheadphonesareaslider;
static struct widget volumeheadphonestext;
static struct widget volumeheadphonesslider;

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
        volumeheadphonetext.payload.text.color = TEXT_COLOR_NORMAL;
        volumeheadphoneslider.payload.slider.value = strtol(data, 0, 10);

        break;

    case 4:
        volumeheadphonestext.payload.text.color = TEXT_COLOR_NORMAL;
        volumeheadphonesslider.payload.slider.value = strtol(data, 0, 10);

        break;

    }

}

static void button(unsigned int key)
{

    selection_move(&view.selection, key);

    switch (key)
    {

    case KEY_LEFT:
        if (selection_isactive(&view.selection, "volumemasterarea"))
        {

            session_create("settings_volume_decrement", 1, ondata, 0, 0);
            session_setarg("settings_volume_decrement", 1, 0, "./helper.sh");
            session_setarg("settings_volume_decrement", 1, 1, "volume_decrement");
            session_setarg("settings_volume_decrement", 1, 2, "Master");
            session_setarg("settings_volume_decrement", 1, 3, 0);
            session_run();

        }

        if (selection_isactive(&view.selection, "volumepcmarea"))
        {

            session_create("settings_volume_decrement", 2, ondata, 0, 0);
            session_setarg("settings_volume_decrement", 2, 0, "./helper.sh");
            session_setarg("settings_volume_decrement", 2, 1, "volume_decrement");
            session_setarg("settings_volume_decrement", 2, 2, "PCM");
            session_setarg("settings_volume_decrement", 2, 3, 0);
            session_run();

        }

        if (selection_isactive(&view.selection, "volumeheadphonearea"))
        {

            session_create("settings_volume_decrement", 3, ondata, 0, 0);
            session_setarg("settings_volume_decrement", 3, 0, "./helper.sh");
            session_setarg("settings_volume_decrement", 3, 1, "volume_decrement");
            session_setarg("settings_volume_decrement", 3, 2, "Headphone");
            session_setarg("settings_volume_decrement", 3, 3, 0);
            session_run();

        }

        if (selection_isactive(&view.selection, "volumeheadphonesarea"))
        {

            session_create("settings_volume_decrement", 4, ondata, 0, 0);
            session_setarg("settings_volume_decrement", 4, 0, "./helper.sh");
            session_setarg("settings_volume_decrement", 4, 1, "volume_decrement");
            session_setarg("settings_volume_decrement", 4, 2, "Headphones");
            session_setarg("settings_volume_decrement", 4, 3, 0);
            session_run();

        }

        break;

    case KEY_RIGHT:
        if (selection_isactive(&view.selection, "volumemasterarea"))
        {

            session_create("settings_volume_increment", 1, ondata, 0, 0);
            session_setarg("settings_volume_increment", 1, 0, "./helper.sh");
            session_setarg("settings_volume_increment", 1, 1, "volume_increment");
            session_setarg("settings_volume_increment", 1, 2, "Master");
            session_setarg("settings_volume_increment", 1, 3, 0);
            session_run();

        }

        if (selection_isactive(&view.selection, "volumepcmarea"))
        {

            session_create("settings_volume_increment", 2, ondata, 0, 0);
            session_setarg("settings_volume_increment", 2, 0, "./helper.sh");
            session_setarg("settings_volume_increment", 2, 1, "volume_increment");
            session_setarg("settings_volume_increment", 2, 2, "PCM");
            session_setarg("settings_volume_increment", 2, 3, 0);
            session_run();

        }

        if (selection_isactive(&view.selection, "volumeheadphonearea"))
        {

            session_create("settings_volume_increment", 3, ondata, 0, 0);
            session_setarg("settings_volume_increment", 3, 0, "./helper.sh");
            session_setarg("settings_volume_increment", 3, 1, "volume_increment");
            session_setarg("settings_volume_increment", 3, 2, "Headphone");
            session_setarg("settings_volume_increment", 3, 3, 0);
            session_run();

        }

        if (selection_isactive(&view.selection, "volumeheadphonesarea"))
        {

            session_create("settings_volume_increment", 4, ondata, 0, 0);
            session_setarg("settings_volume_increment", 4, 0, "./helper.sh");
            session_setarg("settings_volume_increment", 4, 1, "volume_increment");
            session_setarg("settings_volume_increment", 4, 2, "Headphones");
            session_setarg("settings_volume_increment", 4, 3, 0);
            session_run();

        }

        break;

    }

    selection_unselect(&view.selection, key, view.name);

}

static void load(void)
{

    volumemastertext.payload.text.color = TEXT_COLOR_DISABLE;
    volumemasterslider.payload.slider.value = -1;
    volumepcmtext.payload.text.color = TEXT_COLOR_DISABLE;
    volumepcmslider.payload.slider.value = -1;
    volumeheadphonetext.payload.text.color = TEXT_COLOR_DISABLE;
    volumeheadphoneslider.payload.slider.value = -1;
    volumeheadphonestext.payload.text.color = TEXT_COLOR_DISABLE;
    volumeheadphonesslider.payload.slider.value = -1;

    session_create("settings_volume_get", 1, ondata, 0, 0);
    session_setarg("settings_volume_get", 1, 0, "./helper.sh");
    session_setarg("settings_volume_get", 1, 1, "volume_get");
    session_setarg("settings_volume_get", 1, 2, "Master");
    session_setarg("settings_volume_get", 1, 3, 0);
    session_create("settings_volume_get", 2, ondata, 0, 0);
    session_setarg("settings_volume_get", 2, 0, "./helper.sh");
    session_setarg("settings_volume_get", 2, 1, "volume_get");
    session_setarg("settings_volume_get", 2, 2, "PCM");
    session_setarg("settings_volume_get", 2, 3, 0);
    session_create("settings_volume_get", 3, ondata, 0, 0);
    session_setarg("settings_volume_get", 3, 0, "./helper.sh");
    session_setarg("settings_volume_get", 3, 1, "volume_get");
    session_setarg("settings_volume_get", 3, 2, "Headphone");
    session_setarg("settings_volume_get", 3, 3, 0);
    session_create("settings_volume_get", 4, ondata, 0, 0);
    session_setarg("settings_volume_get", 4, 0, "./helper.sh");
    session_setarg("settings_volume_get", 4, 1, "volume_get");
    session_setarg("settings_volume_get", 4, 2, "Headphones");
    session_setarg("settings_volume_get", 4, 3, 0);
    session_run();
    selection_reset(&view.selection);

}

void view_settings_audio_setup(void)
{

    widget_area_init(&audioarea, "", "main", 0, 0, 8, 1);
    widget_text_init(&audiotext, "", "main", TEXT_COLOR_TITLE, TEXT_ALIGN_CENTER, "Volume");
    widget_area_init(&volumemasterarea, "", "main", 0, 1, 8, 1);
    widget_area_init(&volumemasterareatext, "volumemasterareatext", "main", 0, 1, 5, 1);
    widget_area_init(&volumemasterareaslider, "volumemasterareaslider", "main", 5, 1, 3, 1);
    widget_text_init(&volumemastertext, "", "volumemasterareatext", TEXT_COLOR_DISABLE, TEXT_ALIGN_LEFT, "Master");
    widget_slider_init(&volumemasterslider, "", "volumemasterareaslider", 0, 100, -1);
    widget_area_init(&volumepcmarea, "", "main", 0, 2, 8, 1);
    widget_area_init(&volumepcmareatext, "volumepcmareatext", "main", 0, 2, 5, 1);
    widget_area_init(&volumepcmareaslider, "volumepcmareaslider", "main", 5, 2, 3, 1);
    widget_text_init(&volumepcmtext, "", "volumepcmareatext", TEXT_COLOR_DISABLE, TEXT_ALIGN_LEFT, "PCM");
    widget_slider_init(&volumepcmslider, "", "volumepcmareaslider", 0, 100, -1);
    widget_area_init(&volumeheadphonearea, "", "main", 0, 3, 8, 1);
    widget_area_init(&volumeheadphoneareatext, "volumeheadphoneareatext", "main", 0, 3, 5, 1);
    widget_area_init(&volumeheadphoneareaslider, "volumeheadphoneareaslider", "main", 5, 3, 3, 1);
    widget_text_init(&volumeheadphonetext, "", "volumeheadphoneareatext", TEXT_COLOR_DISABLE, TEXT_ALIGN_LEFT, "Headphone");
    widget_slider_init(&volumeheadphoneslider, "", "volumeheadphoneareaslider", 0, 100, -1);
    widget_area_init(&volumeheadphonesarea, "", "main", 0, 4, 8, 1);
    widget_area_init(&volumeheadphonesareatext, "volumeheadphonesareatext", "main", 0, 4, 5, 1);
    widget_area_init(&volumeheadphonesareaslider, "volumeheadphonesareaslider", "main", 5, 4, 3, 1);
    widget_text_init(&volumeheadphonestext, "", "volumeheadphonesareatext", TEXT_COLOR_DISABLE, TEXT_ALIGN_LEFT, "Headphones");
    widget_slider_init(&volumeheadphonesslider, "", "volumeheadphonesareaslider", 0, 100, -1);
    selection_add(&view.selection, &volumemasterarea);
    selection_add(&view.selection, &volumepcmarea);
    selection_add(&view.selection, &volumeheadphonearea);
    selection_add(&view.selection, &volumeheadphonesarea);
    view_init(&view, "settings_audio", load, 0, 0, button);
    view_register(&view, &audioarea);
    view_register(&view, &audiotext);
    view_register(&view, &volumemasterarea);
    view_register(&view, &volumemasterareatext);
    view_register(&view, &volumemasterareaslider);
    view_register(&view, &volumemastertext);
    view_register(&view, &volumemasterslider);
    view_register(&view, &volumepcmarea);
    view_register(&view, &volumepcmareatext);
    view_register(&view, &volumepcmareaslider);
    view_register(&view, &volumepcmtext);
    view_register(&view, &volumepcmslider);
    view_register(&view, &volumeheadphonearea);
    view_register(&view, &volumeheadphoneareatext);
    view_register(&view, &volumeheadphoneareaslider);
    view_register(&view, &volumeheadphonetext);
    view_register(&view, &volumeheadphoneslider);
    view_register(&view, &volumeheadphonesarea);
    view_register(&view, &volumeheadphonesareatext);
    view_register(&view, &volumeheadphonesareaslider);
    view_register(&view, &volumeheadphonestext);
    view_register(&view, &volumeheadphonesslider);
    main_register(&view);

}

