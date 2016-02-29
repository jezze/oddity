#include <stdlib.h>
#include <string.h>
#include "view.h"
#include "box.h"
#include "text.h"
#include "backend.h"

static unsigned int getwordlength(char *text, unsigned int count)
{

    unsigned int i;
    unsigned int total = 0;

    for (i = 0; i < count; i++)
    {

        int advance;

        if (text[i] == ' ' || text[i] == '\n')
            break;

        backend_getmetrics(text[i], NULL, NULL, NULL, NULL, &advance);

        total += advance;

    }

    return total;

}

void text_render(struct text *text, int x, int y, int w, int h, unsigned int color)
{

    int ascent = backend_getascent();
    int totallength = strlen(text->content);
    unsigned int rx = x + TEXT_XPADDING;
    unsigned int ry = y + TEXT_YPADDING;
    unsigned int rw = w - TEXT_XPADDING * 2;
    unsigned int rh = h - TEXT_YPADDING * 2;
    unsigned int gx = rx;
    unsigned int gy = ry;
    unsigned int gw = rw;
    unsigned int gh = rh;
    unsigned int offsety = ry;
    unsigned int i;

    for (i = 0; i < totallength; i++)
    {

        int minx;
        int maxx;
        int miny;
        int maxy;
        int advance;

        if (text->content[i] == '\n')
        {

            gx = rx;
            offsety += 16;

            continue;

        }

        if (gx == rx && text->content[i] == ' ')
            continue;

        if (text->content[i] != ' ')
        {

            int x = getwordlength(&text->content[i], totallength - i - 1);

            if (gx + x > rx + rw)
            {

                gx = rx;
                offsety += 16;

            }

        }

        backend_getmetrics(text->content[i], &minx, &maxx, &miny, &maxy, &advance);

        gy = offsety + ascent - maxy;
        gw = advance;

        backend_glyph(text->content[i], gx, gy, gw, gh, color);

        gx += advance;

        if (gx + gw > rx + rw)
        {

            gx = rx;
            offsety += 16;

        }

    }

}

void text_renderbox(struct textbox *textbox, unsigned int color)
{

    text_render(&textbox->text, textbox->box.x + RENDER_PADDING, textbox->box.y + RENDER_PADDING, textbox->box.w - (2 * RENDER_PADDING), textbox->box.h - (2 * RENDER_PADDING), color);

}

