#include <stdlib.h>
#include <string.h>
#include "config.h"
#include "box.h"
#include "list.h"
#include "widget.h"
#include "view.h"
#include "backend.h"

static unsigned int wordstart(const char *text, unsigned int count)
{

    unsigned int i;

    for (i = count; i > 0; i--)
    {

        if (text[i - 1] == ' ')
            break;

    }

    return i;

}

static unsigned int textw(const char *text, unsigned int count)
{

    unsigned int total = 0;
    unsigned int i;

    for (i = 0; i < count; i++)
    {

        int advance;

        backend_font_getmetrics("default", text[i], NULL, NULL, NULL, NULL, &advance);

        total += advance;

    }

    return total;

}

static unsigned int maxfit(const char *text, unsigned int count, unsigned int width)
{

    unsigned int total = 0;
    unsigned int i;

    for (i = 0; i < count; i++)
    {

        int advance;

        if (text[i] == '\n')
            break;

        backend_font_getmetrics("default", text[i], NULL, NULL, NULL, NULL, &advance);

        total += advance;

        if (total >= width)
            return wordstart(text, i);

    }

    return i;

}

static void renderline(const char *id, const char *text, unsigned int count, unsigned int x, unsigned int y, unsigned int color)
{

    unsigned int i;

    for (i = 0; i < count; i++)
    {

        int minx;
        int maxx;
        int miny;
        int maxy;
        int advance;

        backend_font_getmetrics(id, text[i], &minx, &maxx, &miny, &maxy, &advance);
        backend_paint_glyph(id, text[i], x + minx, y - maxy, maxx - minx, maxy - miny, color);

        x += advance;

    }

}

void text_render(const char *id, const struct box *box, unsigned int color, unsigned int align, const char *content)
{

    const char *ptext = content;
    unsigned int pcount = strlen(content);
    unsigned int liney = box->y + backend_font_getascent("default");
    unsigned int colors[] = {
        COLOR_SHADOW,
        COLOR_NORMAL,
        COLOR_TITLE,
        COLOR_SELECT,
        COLOR_DISABLE
    };

    while (pcount)
    {

        unsigned int linecount = maxfit(ptext, pcount, box->w);
        unsigned int linex;

        switch (align)
        {

        case TEXT_ALIGN_LEFT:
            linex = box->x;

            break;

        case TEXT_ALIGN_RIGHT:
            linex = box->x + box->w - textw(ptext, linecount);

            break;

        case TEXT_ALIGN_CENTER:
            linex = box->x + (box->w - textw(ptext, linecount)) / 2;

            break;

        }

        renderline(id, ptext, linecount, linex, liney, colors[color]);

        liney += TEXT_ROWHEIGHT;
        ptext += linecount;
        pcount -= linecount;

        if (pcount && ptext[0] == '\n')
        {

            ptext++;
            pcount--;

        }

        if (liney > box->y + box->h)
            break;

    }

}

