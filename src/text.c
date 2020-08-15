#include <stdlib.h>
#include <string.h>
#include "define.h"
#include "box.h"
#include "text.h"
#include "backend.h"

#define XPADDING                        12
#define YPADDING                        4

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

        backend_font_getmetrics(text[i], NULL, NULL, NULL, NULL, &advance);

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

        backend_font_getmetrics(text[i], NULL, NULL, NULL, NULL, &advance);

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

        backend_font_getmetrics(text[i], &minx, &maxx, &miny, &maxy, &advance);
        backend_paint_glyph(text[i], x + minx, y - maxy, maxx - minx, maxy - miny, color);

        x += advance;

    }

}

void text_render(struct box *box, unsigned int color, unsigned int align, char *content)
{

    char *ptext = content;
    unsigned int pcount = strlen(content);
    unsigned int x = box->x + XPADDING;
    unsigned int y = box->y + YPADDING;
    unsigned int w = box->w - XPADDING * 2;
    unsigned int h = box->h - YPADDING * 2;
    unsigned int liney = y + backend_font_getascent();

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

void text_render2(struct box *box, unsigned int color, unsigned int align, char *content)
{

    char *ptext = content;
    unsigned int pcount = strlen(content);
    unsigned int liney = box->y + backend_font_getascent();

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

        renderline(ptext, linecount, linex, liney, color);

        while (ptext[linecount] == ' ' || ptext[linecount] == '\n')
        {

            if (ptext[linecount] == '\n')
                liney += 18;

            linecount++;

        }

        ptext += linecount;
        pcount -= linecount;

        if (liney > box->y + box->h)
            break;

    }

}

