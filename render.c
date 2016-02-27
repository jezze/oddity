#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_gfxPrimitives.h>
#include "ztore.h"
#include "text.h"
#include "menu.h"
#include "render.h"

TTF_Font *font;

void render_image(SDL_Surface *image, SDL_Surface *display)
{

    SDL_BlitSurface(image, NULL, display, NULL);

}

static unsigned int getwordlength(char *text, unsigned int count)
{

    unsigned int i;
    unsigned int total = 0;

    for (i = 0; i < count; i++)
    {

        int advance;

        if (text[i] == ' ' || text[i] == '\n')
            break;

        TTF_GlyphMetrics(font, text[i], NULL, NULL, NULL, NULL, &advance);

        total += advance;

    }

    return total;

}

void render_text(struct text *text, SDL_Surface *display, SDL_Rect rect, SDL_Color color)
{

    SDL_Surface *surface;
    SDL_Rect glyphrect;
    unsigned int i;
    unsigned int offsety;
    int ascent = TTF_FontAscent(font);
    int totallength = strlen(text->content);

    rect.x = rect.x + TEXT_XPADDING;
    rect.y = rect.y + TEXT_YPADDING;
    rect.w = rect.w - TEXT_XPADDING * 2;
    rect.h = rect.h - TEXT_YPADDING * 2;
    glyphrect.x = rect.x;
    glyphrect.y = rect.y;
    glyphrect.w = rect.w;
    glyphrect.h = rect.h;
    offsety = rect.y;

    for (i = 0; i < totallength; i++)
    {

        int minx;
        int maxx;
        int miny;
        int maxy;
        int advance;

        if (text->content[i] == '\n')
        {

            glyphrect.x = rect.x;
            offsety += 16;

            continue;

        }

        if (glyphrect.x == rect.x && text->content[i] == ' ')
            continue;

        if (text->content[i] != ' ')
        {

            int x = getwordlength(&text->content[i], totallength - i - 1);

            if (glyphrect.x + x > rect.x + rect.w)
            {

                glyphrect.x = rect.x;
                offsety += 16;

            }

        }

        TTF_GlyphMetrics(font, text->content[i], &minx, &maxx, &miny, &maxy, &advance);

        surface = TTF_RenderGlyph_Solid(font, text->content[i], color);
        glyphrect.y = offsety + ascent - maxy;
        glyphrect.w = advance;

        SDL_BlitSurface(surface, NULL, display, &glyphrect);
        SDL_FreeSurface(surface);

        glyphrect.x += advance;

        if (glyphrect.x + glyphrect.w > rect.x + rect.w)
        {

            glyphrect.x = rect.x;
            offsety += 16;

        }

    }

}

void render_textbox(struct textbox *textbox, SDL_Surface *display)
{

    SDL_Color color;
    SDL_Rect rect;

    rect.x = textbox->box.x;
    rect.y = textbox->box.y;
    rect.w = textbox->box.w;
    rect.h = textbox->box.h;
    color.r = 160;
    color.g = 192;
    color.b = 192;

    return render_text(&textbox->text, display, rect, color);

}

void render_menuitem(struct menuitem *menuitem, SDL_Surface *display, SDL_Rect rect)
{

    SDL_Color color;

    if (menuitem->type & MENUITEM_FLAG_BLOCKED)
    {

        color.r = 64;
        color.g = 96;
        color.b = 96;

    }

    else
    {

        color.r = 224;
        color.g = 96;
        color.b = 32;

    }

    if (menuitem->type & MENUITEM_FLAG_SELECTED)
    {

        filledRectAlpha(display, rect.x, rect.y, rect.x + rect.w - 1, rect.y + rect.h - 1, 0xFFFFFF10);
        rectangleColor(display, rect.x, rect.y, rect.x + rect.w - 1, rect.y + rect.h - 1, 0xFFFFFF40);

    }

    render_text(&menuitem->text, display, rect, color);

}

void render_menu(struct menu *menu, SDL_Surface *display)
{

    unsigned int pagerows = (menu->box.h - 2 * MENU_PADDING) / MENU_ROWHEIGHT;
    unsigned int page = (menu->currentitem / pagerows);
    unsigned int rowstart = page * pagerows;
    unsigned int rowend = (rowstart + pagerows) > menu->total ? menu->total : rowstart + pagerows;
    unsigned int row;

    for (row = rowstart; row < rowend; row++)
    {

        SDL_Rect rect;

        rect.x = menu->box.x + MENU_PADDING;
        rect.y = menu->box.y + MENU_PADDING + (row - rowstart) * MENU_ROWHEIGHT;
        rect.w = menu->box.w - 2 * MENU_PADDING;
        rect.h = MENU_ROWHEIGHT;

        render_menuitem(&menu->items[row], display, rect);

    }

}

void render_initfont()
{

    if (TTF_Init() < 0)
        exit(EXIT_FAILURE);

    font = TTF_OpenFont("habbo.ttf", 16);

    if (!font)
        exit(EXIT_FAILURE);

}

void render_destroyfont()
{

    TTF_Quit();

}

