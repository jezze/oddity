#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include "config.h"
#include "box.h"
#include "list.h"
#include "session.h"
#include "file.h"
#include "db.h"
#include "widget.h"
#include "view.h"
#include "pool.h"
#include "backend.h"
#include "main.h"

void view_app_setup(void);
void view_applist_setup(void);
void view_exit_setup(void);
void view_front_setup(void);
void view_repolist_setup(void);
void view_settings_setup(void);
void view_settings_audio_setup(void);
void view_settings_date_setup(void);
void view_settings_datetime_setup(void);
void view_settings_system_setup(void);
void view_settings_time_setup(void);
void view_settings_timezone_setup(void);
void view_sync_setup(void);
void view_sysinfo_setup(void);
static struct view *active;
static unsigned int quit;
static unsigned int ticks;
static struct list views;

static void setup(void)
{

    view_app_setup();
    view_applist_setup();
    view_exit_setup();
    view_front_setup();
    view_repolist_setup();
    view_settings_setup();
    view_settings_audio_setup();
    view_settings_date_setup();
    view_settings_datetime_setup();
    view_settings_system_setup();
    view_settings_time_setup();
    view_settings_timezone_setup();
    view_sync_setup();
    view_sysinfo_setup();

}

static void precheck(struct view *view)
{

    struct widget *child = 0;

    while ((child = pool_widget_next(view, child)))
    {

        if (child->type == WIDGET_TYPE_OPTION)
        {

            struct widget *parent = view_findwidget(view, child->in);

            if (!parent)
                continue;

            if (parent->type != WIDGET_TYPE_SELECT)
                continue;

            if (strcmp(child->payload.option.value, parent->payload.select.value))
                child->hidden = 1;
            else
                child->hidden = 0;

        }

    }

    if (!view->selected)
        view->selected = view_getselectable(view);

}

static void run(void)
{

    unsigned int frametime;

    while (!quit)
    {

        frametime = backend_ticks();

        session_run();
        session_poll();
        backend_poll(ticks);
        precheck(active);

        if (active->onstep)
            active->onstep(ticks);

        backend_render(active, ticks);

        if (backend_ticks() - frametime < TIMELIMIT)
            backend_delay(TIMELIMIT - (backend_ticks() - frametime));

        ticks++;

    }

}

static void init(void)
{

    backend_init();
    backend_loadimage("background", "back.png");
    backend_createbackground("background");
    backend_loadfont("default", "habbo.ttf", 16);
    backend_loadfont("icon", "icofont.ttf", 16);
    backend_loadfont("bigicon", "icofont.ttf", 40);
    backend_loadsample("select", "select.wav");
    backend_loadsample("unselect", "unselect.wav");
    backend_loadsample("click", "click.wav");
    session_setup();

}

static void destroy(void)
{

    backend_unloadsample("select");
    backend_unloadsample("unselect");
    backend_unloadsample("click");
    backend_unloadfont("default");
    backend_unloadfont("icon");
    backend_unloadfont("bigicon");
    backend_destroybackground();
    backend_unloadimage("background");
    backend_destroy();

}

static struct view *findview(const char *name)
{

    struct list_item *current;

    for (current = views.head; current; current = current->next)
    {

        struct view *view = current->data;

        if (!strcmp(view->name, name))
            return view;

    }

    return 0;

}

void main_configview(const char *name, const char *key, const void *value)
{

    struct view *view = findview(name);

    if (!view)
        return;

    if (view->onconfig)
        view->onconfig(key, value);

}

static void loadview(struct view *view, char *parentname, unsigned int type)
{

    view->parentname = parentname;

    if (view->onload)
        view->onload(type);

    active = view;

}

static void initview(const char *name, char *parentname)
{

    struct view *view = findview(name);

    if (!view)
        return;

    loadview(view, parentname, VIEW_LOADTYPE_INIT);

    view->selected = view_getselectable(view);

}

static void destroyview(const char *name)
{

    struct view *view = findview(name);

    if (!view)
        return;

    if (view->parentname)
    {

        struct view *parent = findview(view->parentname);

        if (!parent)
            return;

        loadview(parent, parent->parentname, VIEW_LOADTYPE_RESTORE);

    }

}

void main_registerview(struct view *view)
{

    list_add(&views, &view->item);

}

void main_exec(const char id[6], const char *sha1)
{

    char name[14];
    char opk[64];
    char *argv[3];
    unsigned int i;
    unsigned int k = 0;

    for (i = 0; i < 6; i++)
        name[k++] = tolower(id[i]);

    name[k++] = '.';

    for (i = 0; i < 7; i++)
        name[k++] = sha1[i];

    snprintf(opk, 64, "/media/data/apps/%s.opk", name);

    argv[0] = "opkrun";
    argv[1] = opk;
    argv[2] = 0;

    destroy();
    execvp(argv[0], argv);

}

void main_loadview(struct view *view, const char *match, const char *to)
{

    if (!view->selected)
        return;

    if (!strlen(view->selected->id))
        return;

    if (strcmp(view->selected->id, match))
        return;

    initview(to, view->name);
    backend_play("select");

}

void main_unloadview(struct view *view)
{

    if (!view->parentname)
        return;

    destroyview(view->name);
    backend_play("unselect");

}

static void moveselection(struct view *view, unsigned int button)
{

    struct list_item *current;
    struct widget *best = 0;
    int bestdx = 5000;
    int bestdy = 5000;
    int mx;
    int my;

    if (!(button == BUTTON_LEFT || button == BUTTON_RIGHT || button == BUTTON_UP || button == BUTTON_DOWN))
        return;

    if (!view->selected)
        return;

    mx = view->selected->size.x + view->selected->size.w / 2;
    my = view->selected->size.y + view->selected->size.h / 2;

    for (current = view->widgets.head; current; current = current->next)
    {

        struct widget *widget = current->data;
        int dx = abs(widget->size.x - view->selected->size.x);
        int dy = abs(widget->size.y - view->selected->size.y);

        if (!widget->selectable)
            continue;

        switch (button)
        {

        case BUTTON_LEFT:
            if (widget->size.x + widget->size.w >= mx)
                continue;

            if (dy <= bestdy && dx <= bestdx)
            {

                best = widget;
                bestdx = dx;
                bestdy = dy;

            }

            break;

        case BUTTON_RIGHT:
            if (widget->size.x < mx)
                continue;

            if (dy <= bestdy && dx <= bestdx)
            {

                best = widget;
                bestdx = dx;
                bestdy = dy;

            }

            break;

        case BUTTON_UP:
            if (widget->size.y + widget->size.h >= my)
                continue;

            if (dx <= bestdx && dy <= bestdy)
            {

                best = widget;
                bestdx = dx;
                bestdy = dy;

            }

            break;

        case BUTTON_DOWN:
            if (widget->size.y < my)
                continue;

            if (dx <= bestdx && dy <= bestdy)
            {

                best = widget;
                bestdx = dx;
                bestdy = dy;

            }

            break;

        }

    }

    if (best)
    {

        view->selected = best;

        backend_play("click");

    }

}

void main_goprev(const struct view *view, const char *id)
{

    struct widget *widget = view_findwidget(view, id);
    const struct widget *child = 0;

    if (!widget)
        return;

    while ((child = pool_widget_prevchild(view, child, widget)))
    {

        if (!strcmp(widget->payload.select.value, child->payload.option.value))
            break;

    }

    if (child)
    {

        child = pool_widget_prevchild(view, child, widget);

        if (child)
            view_setattr(view, id, "value", child->payload.option.value);

    }

}

void main_gonext(const struct view *view, const char *id)
{

    struct widget *widget = view_findwidget(view, id);
    const struct widget *child = 0;

    if (!widget)
        return;

    while ((child = pool_widget_nextchild(view, child, widget)))
    {

        if (!strcmp(widget->payload.select.value, child->payload.option.value))
            break;

    }

    if (child)
    {

        child = pool_widget_nextchild(view, child, widget);

        if (child)
            view_setattr(view, id, "value", child->payload.option.value);

    }

}

void main_button(unsigned int button)
{

    moveselection(active, button);

    if (button == BUTTON_B)
        main_unloadview(active);

    if (active->onbutton)
        active->onbutton(button);

}

void main_quit(void)
{

    quit = 1;

}

int main(int argc, char **argv)
{

    init();
    setup();
    file_init();
    db_init();
    initview("front", 0);
    run();
    destroyview("front");
    destroy();

    return 0;

}

