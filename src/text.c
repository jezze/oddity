#include <stdlib.h>
#include <string.h>
#include "define.h"
#include "box.h"
#include "text.h"
#include "view.h"
#include "backend.h"

static unsigned int rtrim(char *text, unsigned int count)
{

    unsigned int i;

    for (i = count; i > 0; i--)
    {

        if (text[i - 1] == ' ')
            break;

    }

    return i - 1;

}

static unsigned int textw(char *text, unsigned int count)
{

    unsigned int i;
    unsigned int total = 0;

    for (i = 0; i < count; i++)
    {

        int advance;

        backend_getmetrics(text[i], NULL, NULL, NULL, NULL, &advance);

        total += advance;

    }

    return total;

}

static unsigned int maxfit(char *text, unsigned int count, unsigned int width)
{

    unsigned int i;
    unsigned int total = 0;

    for (i = 0; i < count; i++)
    {

        int advance;

        if (text[i] == '\n')
            break;

        backend_getmetrics(text[i], NULL, NULL, NULL, NULL, &advance);

        total += advance;

        if (total >= width)
            return rtrim(text, i);

    }

    return i;

}

static void renderline(char *text, unsigned int count, unsigned int x, unsigned int y, unsigned int color)
{

    unsigned int i;

    for (i = 0; i < count; i++)
    {

        int minx;
        int maxx;
        int miny;
        int maxy;
        int advance;

        backend_getmetrics(text[i], &minx, &maxx, &miny, &maxy, &advance);
        backend_glyph(text[i], x + minx, y - maxy, maxx - minx, maxy - miny, color);

        x += advance;

    }

}

void text_render(struct text *text, int x, int y, int w, int h, unsigned int color, unsigned int align, char *content)
{

    char *ptext = content;
    unsigned int pcount = strlen(content);
    unsigned int liney;

    x += TEXT_XPADDING;
    y += TEXT_YPADDING;
    w -= TEXT_XPADDING * 2;
    h -= TEXT_YPADDING * 2;
    liney = y + backend_getascent();

    while (pcount)
    {

        unsigned int linecount = maxfit(ptext, pcount, w);
        unsigned int linex;

        switch (align)
        {

        case TEXT_ALIGN_LEFT:
            linex = x;

            break;

        case TEXT_ALIGN_RIGHT:
            linex = x + w - textw(ptext, linecount);

            break;

        case TEXT_ALIGN_CENTER:
            linex = x + (w - textw(ptext, linecount)) / 2;

            break;

        }

        renderline(ptext, linecount, linex, liney, color);

        while (ptext[linecount] == ' ' || ptext[linecount] == '\n')
        {

            if (ptext[linecount] == '\n')
                liney += 16;

            linecount++;

        }

        ptext += linecount;
        pcount -= linecount;

        if (liney > y + h)
            break;

    }

}

void text_renderbox(struct textbox *textbox, int x, int y, int w, int h, unsigned int color, unsigned int align, char *content)
{

    text_render(&textbox->text, x + RENDER_PADDING, y + RENDER_PADDING, w - (2 * RENDER_PADDING), h - (2 * RENDER_PADDING), color, align, content);

}

void text_init(struct text *text)
{

}

