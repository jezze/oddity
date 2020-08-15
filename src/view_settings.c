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
static struct widget_area audio_area;
static struct widget_text audio_text;
static struct widget_area areas[3];
static struct widget_area volume_master_area_text;
static struct widget_area volume_master_area_slider;
static struct widget_text volume_master_text;
static struct widget_slider volume_master_slider;
static struct widget_area volume_pcm_area_text;
static struct widget_area volume_pcm_area_slider;
static struct widget_text volume_pcm_text;
static struct widget_slider volume_pcm_slider;
static struct widget_area volume_headphones_area_text;
static struct widget_area volume_headphones_area_slider;
static struct widget_text volume_headphones_text;
static struct widget_slider volume_headphones_slider;
static struct selection selection;

static void place(unsigned int w, unsigned int h)
{

    widget_area_place(&audio_area, 0, 0, w, h);
    widget_text_placein(&audio_text, &audio_area.size);
    widget_area_place(&areas[0], 0, 0, w, h);
    widget_area_place(&areas[1], 0, 0, w, h);
    widget_area_place(&areas[2], 0, 0, w, h);
    widget_area_place(&volume_master_area_text, 0, 0, w, h);
    widget_area_place(&volume_master_area_slider, 0, 0, w, h);
    widget_text_placein(&volume_master_text, &volume_master_area_text.size);
    widget_slider_placein(&volume_master_slider, &volume_master_area_slider.size);
    widget_area_place(&volume_pcm_area_text, 0, 0, w, h);
    widget_area_place(&volume_pcm_area_slider, 0, 0, w, h);
    widget_text_placein(&volume_pcm_text, &volume_pcm_area_text.size);
    widget_slider_placein(&volume_pcm_slider, &volume_pcm_area_slider.size);
    widget_area_place(&volume_headphones_area_text, 0, 0, w, h);
    widget_area_place(&volume_headphones_area_slider, 0, 0, w, h);
    widget_text_placein(&volume_headphones_text, &volume_headphones_area_text.size);
    widget_slider_placein(&volume_headphones_slider, &volume_headphones_area_slider.size);

}

static void render(unsigned int ticks)
{

    selection_render(&selection, ticks);
    widget_text_render(&audio_text, ticks);
    widget_text_render(&volume_master_text, ticks);
    widget_slider_render(&volume_master_slider, ticks);
    widget_text_render(&volume_pcm_text, ticks);
    widget_slider_render(&volume_pcm_slider, ticks);
    widget_text_render(&volume_headphones_text, ticks);
    widget_slider_render(&volume_headphones_slider, ticks);

}

void ondata(unsigned int id, void *data, unsigned int count)
{

    switch (id)
    {

    case 1:
        volume_master_text.color = TEXT_COLOR_NORMAL;
        volume_master_slider.value = strtol(data, 0, 10);

        break;

    case 2:
        volume_pcm_text.color = TEXT_COLOR_NORMAL;
        volume_pcm_slider.value = strtol(data, 0, 10);

        break;

    case 3:
        volume_headphones_text.color = TEXT_COLOR_NORMAL;
        volume_headphones_slider.value = strtol(data, 0, 10);

        break;

    }

}

void oncomplete(unsigned int id)
{

    switch (id)
    {

    case 1:
        break;

    case 2:
        break;

    case 3:
        break;

    }

}

void onfailure(unsigned int id)
{

}

static void button(unsigned int key)
{

    selection_setclosest(&selection, key);

    switch (key)
    {

    case KEY_LEFT:
        session_create("settings_volume_decrement", 2, ondata, oncomplete, onfailure);
        session_setarg("settings_volume_decrement", 0, "./helper.sh");
        session_setarg("settings_volume_decrement", 1, "volume_decrement");
        session_setarg("settings_volume_decrement", 2, "PCM");
        session_setarg("settings_volume_decrement", 3, 0);
        session_run();

        break;

    case KEY_RIGHT:
        session_create("settings_volume_increment", 2, ondata, oncomplete, onfailure);
        session_setarg("settings_volume_increment", 0, "./helper.sh");
        session_setarg("settings_volume_increment", 1, "volume_increment");
        session_setarg("settings_volume_increment", 2, "PCM");
        session_setarg("settings_volume_increment", 3, 0);
        session_run();

        break;

    }

    selection_unselect(&selection, key, "settings");

}

static void load(void)
{

    session_create("settings_volume_get_master", 1, ondata, oncomplete, onfailure);
    session_setarg("settings_volume_get_master", 0, "./helper.sh");
    session_setarg("settings_volume_get_master", 1, "volume_get");
    session_setarg("settings_volume_get_master", 2, "Master");
    session_setarg("settings_volume_get_master", 3, 0);
    session_create("settings_volume_get_pcm", 2, ondata, oncomplete, onfailure);
    session_setarg("settings_volume_get_pcm", 0, "./helper.sh");
    session_setarg("settings_volume_get_pcm", 1, "volume_get");
    session_setarg("settings_volume_get_pcm", 2, "PCM");
    session_setarg("settings_volume_get_pcm", 3, 0);
    session_create("settings_volume_get_headphones", 3, ondata, oncomplete, onfailure);
    session_setarg("settings_volume_get_headphones", 0, "./helper.sh");
    session_setarg("settings_volume_get_headphones", 1, "volume_get");
    session_setarg("settings_volume_get_headphones", 2, "Headphones");
    session_setarg("settings_volume_get_headphones", 3, 0);
    session_run();
    main_setview(place, render, button);
    selection_reset(&selection);

}

void view_settings_setup(void)
{

    view_init(&view, "settings", load, 0);
    view_register(&view);
    widget_area_init(&audio_area, 0, 0, 8, 1);
    widget_text_init(&audio_text, TEXT_COLOR_TITLE, TEXT_ALIGN_LEFT, "Audio");
    widget_area_init(&areas[0], 0, 1, 8, 1);
    selection_add(&selection, &areas[0].item);
    widget_area_init(&areas[1], 0, 2, 8, 1);
    selection_add(&selection, &areas[1].item);
    widget_area_init(&areas[2], 0, 3, 8, 1);
    selection_add(&selection, &areas[2].item);

    widget_area_init(&volume_master_area_text, 0, 1, 5, 1);
    widget_area_init(&volume_master_area_slider, 5, 1, 3, 1);
    widget_text_init(&volume_master_text, TEXT_COLOR_DISABLE, TEXT_ALIGN_LEFT, "Volume (Master)");
    widget_slider_init(&volume_master_slider, 0, 100, -1);

    widget_area_init(&volume_pcm_area_text, 0, 2, 5, 1);
    widget_area_init(&volume_pcm_area_slider, 5, 2, 3, 1);
    widget_text_init(&volume_pcm_text, TEXT_COLOR_DISABLE, TEXT_ALIGN_LEFT, "Volume (PCM)");
    widget_slider_init(&volume_pcm_slider, 0, 100, -1);

    widget_area_init(&volume_headphones_area_text, 0, 3, 5, 1);
    widget_area_init(&volume_headphones_area_slider, 5, 3, 3, 1);
    widget_text_init(&volume_headphones_text, TEXT_COLOR_DISABLE, TEXT_ALIGN_LEFT, "Volume (Headphones)");
    widget_slider_init(&volume_headphones_slider, 0, 100, -1);

}

