#include <stdlib.h>
#include "view.h"
#include "box.h"
#include "text.h"
#include "menu.h"
#include "backend.h"
#include "db.h"
#include "ztore.h"

static struct view view;
static struct textbox text;

static void show()
{

    db_init();
    db_sync();

}

static void hide()
{

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
        ztore_flipview(view.parent);

        break;

    }

}

struct view *view_sync_setup(unsigned int w, unsigned int h)
{

    view_init(&view, show, hide, render, keydown);
    text_init(&text.text, "Sync complete!");
    box_init(&text.box, 0, 0, w, (4 * RENDER_ROWHEIGHT) + (2 * RENDER_PADDING));

    return &view;

}

