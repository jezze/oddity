#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include "ztore.h"
#include "event.h"
#include "text.h"
#include "render.h"

SDL_Surface *display;
SDL_Surface *background;
SDL_Surface *blur;
TTF_Font *font;

static void blit(SDL_Surface *surface, int x, int y, int w, int h)
{

    SDL_Rect r;

    r.x = x;
    r.y = y;
    r.w = w;
    r.h = h;

    SDL_BlitSurface(surface, NULL, display, &r);

}

int render_getascent()
{

    return TTF_FontAscent(font);

}

void render_getmetrics(char c, int *minx, int *maxx, int *miny, int *maxy, int *advance)
{

    TTF_GlyphMetrics(font, c, minx, maxx, miny, maxy, advance);

}

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

void render_glyph(char c, unsigned int x, unsigned int y, unsigned int w, unsigned int h, int r, int g, int b)
{

    SDL_Surface *surface;
    SDL_Rect rect;
    SDL_Color color;

    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;
    color.r = r;
    color.g = g;
    color.b = b;
    surface = TTF_RenderGlyph_Solid(font, c, color);

    SDL_BlitSurface(surface, NULL, display, &rect);
    SDL_FreeSurface(surface);

}

void render_text(struct text *text, int x, int y, int w, int h, int r, int g, int b)
{

    int ascent = render_getascent();
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

        render_getmetrics(text->content[i], &minx, &maxx, &miny, &maxy, &advance);

        gy = offsety + ascent - maxy;
        gw = advance;

        render_glyph(text->content[i], gx, gy, gw, gh, r, g, b);

        gx += advance;

        if (gx + gw > rx + rw)
        {

            gx = rx;
            offsety += 16;

        }

    }

}

void render_textbox(struct textbox *textbox, int r, int g, int b)
{

    render_text(&textbox->text, textbox->box.x + RENDER_PADDING, textbox->box.y + RENDER_PADDING, textbox->box.w - (2 * RENDER_PADDING), textbox->box.h - (2 * RENDER_PADDING), r, g, b);

}

static void dofillrectangle(SDL_Surface *s, int w, int h, unsigned int color)
{

    unsigned int *p = s->pixels;
    unsigned int i;

    for (i = 0; i < w * h; i++)
        p[i] = color;

}

static void doborderrectangle(SDL_Surface *s, int w, int h, unsigned int color)
{

    unsigned int *p = s->pixels;
    unsigned int i;

    for (i = 0; i < w; i++)
    {

        p[i] = color;
        p[(h - 1) * w + i] = color;

    }

    for (i = 0; i < h; i++)
    {

        p[i * w] = color;
        p[i * w + (w - 1)] = color;

    }

}

void render_rect(int x, int y, int w, int h)
{

    SDL_Surface *surface = SDL_CreateRGBSurface(0, w, h, 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);

    dofillrectangle(surface, w, h, 0x10FFFFFF);
    doborderrectangle(surface, w, h, 0x40FFFFFF);
    blit(surface, x, y, w, h);

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
        view->onkey(event.key.keysym.sym);

        break;

    case SDL_QUIT:
        event_quit();

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

