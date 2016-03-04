#include <stdlib.h>
#include "define.h"
#include "box.h"
#include "text.h"
#include "menu.h"
#include "db.h"
#include "view.h"
#include "ztore.h"

static struct view view;
static struct textbox text;

static void show()
{

    db_init();
    db_sync();
    ztore_flipview(&view);

}

static void render()
{

    text_renderbox(&text, TEXT_COLOR_NORMAL);

}

static void keydown(unsigned int key)
{

    switch (key)
    {

    case KEY_B:
        view_quit(&view);

        break;

    }

}

struct view *view_sync_setup(unsigned int w, unsigned int h)
{

    view_init(&view, show, render, keydown);
    text_init(&text.text, "Sync complete!");
    box_init(&text.box, 0, 0, w, (4 * RENDER_ROWHEIGHT) + (2 * RENDER_PADDING));

    return &view;

}

