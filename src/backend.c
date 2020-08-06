#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_thread.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include "define.h"
#include "backend.h"

SDL_Surface *display;
SDL_Surface *background;
SDL_Surface *blur;
TTF_Font *font;
TTF_Font *ofont;

static void blit(SDL_Surface *surface, int x, int y, int w, int h)
{

    SDL_Rect r;

    r.x = x;
    r.y = y;
    r.w = w;
    r.h = h;

    SDL_BlitSurface(surface, NULL, display, &r);

}

int backend_getascent(void)
{

    return TTF_FontAscent(font);

}

void backend_getmetrics(char c, int *minx, int *maxx, int *miny, int *maxy, int *advance)
{

    TTF_GlyphMetrics(font, c, minx, maxx, miny, maxy, advance);

}

void backend_glyph(char c, unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int col)
{

    SDL_Surface *surface;
    SDL_Surface *osurface;
    SDL_Rect rect;
    SDL_Rect orect;
    SDL_Color color;
    SDL_Color ocolor;

    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;
    color.r = col >> 16;
    color.g = col >> 8;
    color.b = col >> 0;
    surface = TTF_RenderGlyph_Solid(font, c, color);
    orect.x = x - 1;
    orect.y = y - 1;
    orect.w = w;
    orect.h = h;
    ocolor.r = 10;
    ocolor.g = 10;
    ocolor.b = 10;
    osurface = TTF_RenderGlyph_Solid(ofont, c, ocolor);

    SDL_BlitSurface(osurface, NULL, display, &orect);
    SDL_FreeSurface(osurface);
    SDL_BlitSurface(surface, NULL, display, &rect);
    SDL_FreeSurface(surface);

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

void backend_rect(int x, int y, int w, int h)
{

    SDL_Surface *surface = SDL_CreateRGBSurface(0, w, h, display->format->BitsPerPixel, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

    dofillrectangle(surface, w, h, 0x10FFFFFF);
    doborderrectangle(surface, w, h, 0x40FFFFFF);
    blit(surface, x, y, w, h);

}

static void handleevent(SDL_Event *event, void (*quit)(void), void (*button)(unsigned int key))
{

    switch (event->type)
    {

    case SDL_KEYDOWN:
        switch (event->key.keysym.sym)
        {

        case SDLK_LEFT:
            button(KEY_LEFT);

            break;

        case SDLK_RIGHT:
            button(KEY_RIGHT);

            break;

        case SDLK_UP:
            button(KEY_UP);

            break;

        case SDLK_DOWN:
            button(KEY_DOWN);

            break;

        case SDLK_LCTRL:
            button(KEY_A);

            break;

        case SDLK_LALT:
            button(KEY_B);

            break;

        case SDLK_LSHIFT:
            button(KEY_X);

            break;

        case SDLK_SPACE:
            button(KEY_Y);

            break;

        case SDLK_TAB:
            button(KEY_L);

            break;

        case SDLK_BACKSPACE:
            button(KEY_R);

            break;

        case SDLK_ESCAPE:
            button(KEY_SELECT);

            break;

        case SDLK_RETURN:
            button(KEY_START);

            break;

        case SDLK_PAUSE:
            button(KEY_LOCKDOWN);

            break;

        case SDLK_HOME:
            button(KEY_LOCKUP);

            break;

        default:
            break;

        }

        break;

    case SDL_QUIT:
        quit();

        break;

    }

}

void backend_pollevent(void (*quit)(void), void (*button)(unsigned int key))
{

    SDL_Event event;

    while (SDL_PollEvent(&event))
        handleevent(&event, quit, button);

}

void backend_waitevent(void (*quit)(void), void (*button)(unsigned int key))
{

    SDL_Event event;

    SDL_WaitEvent(&event);
    handleevent(&event, quit, button);

}

void backend_render(void (*render)(void))
{

    SDL_BlitSurface(background, NULL, display, NULL);
    render();
    SDL_Flip(display);

}

void backend_init(unsigned int w, unsigned int h, unsigned int bpp)
{

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
        exit(EXIT_FAILURE);

    display = SDL_SetVideoMode(w, h, bpp, SDL_HWSURFACE | SDL_DOUBLEBUF);

    if (!display)
        exit(EXIT_FAILURE);

    SDL_ShowCursor(SDL_DISABLE);

    if (TTF_Init() < 0)
        exit(EXIT_FAILURE);

}

void backend_destroy(void)
{

    TTF_Quit();
    SDL_Quit();

}

void backend_loadbackground(char *name)
{

    background = IMG_Load(name);

    if (!background)
        exit(EXIT_FAILURE);

    blur = SDL_CreateRGBSurface(0, display->w, display->h, display->format->BitsPerPixel, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

    if (!blur)
        exit(EXIT_FAILURE);

    SDL_FillRect(blur, NULL, SDL_MapRGBA(blur->format, 0x00, 0x00, 0x00, 0xE0));

}

void backend_loadfont(char *name)
{

    font = TTF_OpenFont(name, 16);

    if (!font)
        exit(EXIT_FAILURE);

    ofont = TTF_OpenFont(name, 16);

    if (!ofont)
        exit(EXIT_FAILURE);

    TTF_SetFontOutline(ofont, 1);

}

void backend_unloadbackground(void)
{

    SDL_FreeSurface(background);
    SDL_FreeSurface(blur);

}

void backend_unloadfont(void)
{

    TTF_CloseFont(font);
    TTF_CloseFont(ofont);

}

