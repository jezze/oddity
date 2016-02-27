#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_gfxPrimitives.h>
#include "ztore.h"
#include "text.h"
#include "menu.h"
#include "render.h"

SDL_Surface *display;
SDL_Surface *background;
SDL_Surface *blur;
TTF_Font *font;

void render_background()
{

    SDL_BlitSurface(background, NULL, display, NULL);

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

void render_text(struct text *text, int x, int y, int w, int h, int r, int g, int b)
{

    SDL_Surface *surface;
    SDL_Rect rect;
    SDL_Rect glyphrect;
    SDL_Color color;
    unsigned int i;
    unsigned int offsety;
    int ascent = TTF_FontAscent(font);
    int totallength = strlen(text->content);

    color.r = r;
    color.g = g;
    color.b = b;
    rect.x = x + TEXT_XPADDING;
    rect.y = y + TEXT_YPADDING;
    rect.w = w - TEXT_XPADDING * 2;
    rect.h = h - TEXT_YPADDING * 2;
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

void render_textbox(struct textbox *textbox)
{

    return render_text(&textbox->text, textbox->box.x, textbox->box.y, textbox->box.w, textbox->box.h, 160, 192, 192);

}

void render_menuitem(struct menuitem *menuitem, int x, int y, int w, int h)
{

    if (menuitem->type & MENUITEM_FLAG_SELECTED)
    {

        filledRectAlpha(display, x, y, x + w - 1, y + h - 1, 0xFFFFFF10);
        rectangleColor(display, x, y, x + w - 1, y + h - 1, 0xFFFFFF40);

    }

    if (menuitem->type & MENUITEM_FLAG_BLOCKED)
        render_text(&menuitem->text, x, y, w, h, 64, 96, 96);
    else
        render_text(&menuitem->text, x, y, w, h, 224, 96, 32);

}

void render_menu(struct menu *menu)
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

        render_menuitem(&menu->items[row], rect.x, rect.y, rect.w, rect.h);

    }

}

void render_flip()
{

    SDL_Flip(display);

}

void render_waitevent(struct view *view)
{

    SDL_Event event;

    SDL_WaitEvent(&event);

    switch (event.type)
    {

    case SDL_KEYDOWN:
        view->handlekey(event.key.keysym.sym);

        break;

    case SDL_QUIT:
        ztore_changestate(0);

        break;

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

void render_init()
{

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
        exit(EXIT_FAILURE);

    display = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_HWSURFACE | SDL_DOUBLEBUF);

    if (!display)
        exit(EXIT_FAILURE);

    SDL_ShowCursor(0);

    background = IMG_Load("back.png");

    if (!background)
        exit(EXIT_FAILURE);

    blur = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);

    if (!blur)
        exit(EXIT_FAILURE);

    SDL_FillRect(blur, NULL, SDL_MapRGBA(blur->format, 0x00, 0x00, 0x00, 0xE0));

}

void render_destroyfont()
{

    TTF_Quit();

}

void render_destroy()
{

    SDL_Quit();

}

