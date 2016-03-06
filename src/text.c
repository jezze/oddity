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

static void renderline(char *text, unsigned int count, unsigned int ascent, unsigned int offsety, unsigned int gx, unsigned int gy, unsigned int gw, unsigned int gh, unsigned int color)
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

        gy = offsety + ascent - maxy;
        gw = advance;

        backend_glyph(text[i], gx, gy, gw, gh, color);

        gx += advance;

    }

}

void text_render(struct text *text, int x, int y, int w, int h, unsigned int color, unsigned int align)
{

    int ascent = backend_getascent();
    char *ptext = text->content;
    unsigned int pcount = strlen(text->content);
    unsigned int rx = x + TEXT_XPADDING;
    unsigned int ry = y + TEXT_YPADDING;
    unsigned int rw = w - TEXT_XPADDING * 2;
    unsigned int rh = h - TEXT_YPADDING * 2;
    unsigned int gx = rx;
    unsigned int gy = ry;
    unsigned int gw = rw;
    unsigned int gh = rh;
    unsigned int offsety = ry;

    while (pcount)
    {

        unsigned int linecount = maxfit(ptext, pcount, rw);

        switch (align)
        {

        case TEXT_ALIGN_LEFT:
            gx = rx;

            break;

        case TEXT_ALIGN_RIGHT:
            gx = rx + gw - textw(ptext, linecount);

            break;

        }

        renderline(ptext, linecount, ascent, offsety, gx, gy, gw, gh, color);

        while (ptext[linecount] == ' ' || ptext[linecount] == '\n')
            linecount++;

        offsety += 16;
        ptext += linecount;
        pcount -= linecount;

    }

}

void text_renderbox(struct textbox *textbox, unsigned int color, unsigned int align, char *content)
{

    textbox->text.content = content;

    text_render(&textbox->text, textbox->box.x + RENDER_PADDING, textbox->box.y + RENDER_PADDING, textbox->box.w - (2 * RENDER_PADDING), textbox->box.h - (2 * RENDER_PADDING), color, align);

}

void text_init(struct text *text, char *content)
{

    text->content = content;

}

