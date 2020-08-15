#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_thread.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include "define.h"
#include "backend.h"

struct image
{

    char *name;
    SDL_Surface *surface;

};

struct sample
{

    char *name;
    Mix_Chunk *chunk;

};

static SDL_Surface *display;
static SDL_Surface *background;
static TTF_Font *font;
static TTF_Font *ofont;
static unsigned int ssw;
static unsigned int ssh;
static struct sample samples[8];
static unsigned int nsamples;
static struct image images[8];
static unsigned int nimages;

static void dofillstripes(SDL_Surface *s, int w, int h, unsigned int color, unsigned int ticks)
{

    unsigned int *p = s->pixels;
    unsigned int i = 0;
    unsigned int x;
    unsigned int y;

    for (y = 0; y < h; y++)
    {

        for (x = 0; x < w; x++)
        {

            if (((i + ticks) % 10) > 5)
                p[i] = color;

            i++;

        }

        i += (s->w - w);

    }

}


static void dofillrectangle(SDL_Surface *s, int w, int h, unsigned int color)
{

    unsigned int *p = s->pixels;
    unsigned int i = 0;
    unsigned int x;
    unsigned int y;

    for (y = 0; y < h; y++)
    {

        for (x = 0; x < w; x++)
        {

            p[i] = color;

            i++;

        }

        i += (s->w - w);

    }

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

static void renderbackground(unsigned int ticks)
{

    SDL_Rect src;
    SDL_Rect dst;

    src.x = ticks % ssw;
    src.y = ticks % ssh;
    src.w = SCREEN_WIDTH;
    src.h = SCREEN_HEIGHT;
    dst.x = 0;
    dst.y = 0;
    dst.w = SCREEN_WIDTH;
    dst.h = SCREEN_HEIGHT;

    SDL_BlitSurface(background, &src, display, &dst);

}

static struct image *findimage(char *name)
{

    unsigned int i;

    for (i = 0; i < nimages; i++)
    {

        struct image *image = &images[i];

        if (!strcmp(image->name, name))
            return image;


    }

    return 0;

}

static struct sample *findsample(char *name)
{

    unsigned int i;

    for (i = 0; i < nsamples; i++)
    {

        struct sample *sample = &samples[i];

        if (!strcmp(sample->name, name))
            return sample;


    }

    return 0;

}

int backend_font_getascent(void)
{

    return TTF_FontAscent(font);

}

void backend_font_getmetrics(char c, int *minx, int *maxx, int *miny, int *maxy, int *advance)
{

    int a;

    TTF_GlyphMetrics(font, c, minx, maxx, miny, maxy, &a);

    if (advance)
        *advance = a + 2;

}

void backend_paint_glyph(char c, unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int col)
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

void backend_paint_slider(int x, int y, int w, int h, int ws, unsigned int ticks)
{

    SDL_Surface *surface = SDL_CreateRGBSurface(0, w, h, display->format->BitsPerPixel, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
    SDL_Rect r;

    r.x = x;
    r.y = y;
    r.w = w;
    r.h = h;

    dofillrectangle(surface, w, h, 0x40FFFFFF);

    if (ws >= 0)
    {

        dofillrectangle(surface, ws, h, 0xC0FFFFFF);

    }

    else
    {

        dofillstripes(surface, w, h, 0x60FFFFFF, ticks);

    }

    doborderrectangle(surface, w, h, 0xC0FFFFFF);
    SDL_BlitSurface(surface, NULL, display, &r);
    SDL_FreeSurface(surface);

}

void backend_paint_selection(int x, int y, int w, int h)
{

    SDL_Surface *surface = SDL_CreateRGBSurface(0, w, h, display->format->BitsPerPixel, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
    SDL_Rect r;

    r.x = x;
    r.y = y;
    r.w = w;
    r.h = h;

    dofillrectangle(surface, w, h, 0x10FFFFFF);
    doborderrectangle(surface, w, h, 0x40FFFFFF);
    SDL_BlitSurface(surface, NULL, display, &r);
    SDL_FreeSurface(surface);

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

void backend_render(unsigned int ticks, void (*place)(unsigned int w, unsigned int h), void (*render)(unsigned int ticks))
{

    place(SCREEN_WIDTH, SCREEN_HEIGHT);
    renderbackground(ticks);
    render(ticks);
    SDL_Flip(display);

}

unsigned int backend_ticks(void)
{

    return SDL_GetTicks();

}

void backend_delay(unsigned int ms)
{

    SDL_Delay(ms);

}

void backend_play(char *name)
{

    struct sample *sample = findsample(name);

    if (sample)
        Mix_PlayChannel(-1, sample->chunk, 0);

}

void backend_loadimage(char *name, char *path)
{

    struct image *image = &images[nimages++];

    image->name = name;
    image->surface = IMG_Load(path);

    if (!image->surface)
    {

        fprintf(stderr, "Unable to load chunk: %s\n", IMG_GetError());
        exit(EXIT_FAILURE);

    }

}

void backend_unloadimage(char *name)
{

    struct image *image = findimage(name);

    if (image)
        SDL_FreeSurface(image->surface);

}

void backend_tilebackground(char *name)
{

    struct image *image = findimage(name);
    unsigned int x;
    unsigned int y;

    background = SDL_CreateRGBSurface(0, display->w + image->surface->w, display->h + image->surface->h, display->format->BitsPerPixel, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
    ssw = image->surface->w;
    ssh = image->surface->h;

    for (y = 0; y < background->h; y += image->surface->h)
    {

        for (x = 0; x < background->w; x += image->surface->w)
        {

            SDL_Rect src;
            SDL_Rect dst;

            src.x = 0;
            src.y = 0;
            src.w = image->surface->w;
            src.h = image->surface->h;
            dst.x = x;
            dst.y = y;
            dst.w = image->surface->w;
            dst.h = image->surface->h;

            SDL_BlitSurface(image->surface, &src, background, &dst);

        }

    }

}

void backend_loadsample(char *name, char *path)
{

    struct sample *sample = &samples[nsamples++];

    sample->name = name;
    sample->chunk = Mix_LoadWAV(path);

    if (!sample->chunk)
    {

        fprintf(stderr, "Unable to load chunk: %s\n", Mix_GetError());
        exit(EXIT_FAILURE);

    }

}

void backend_loadfont(char *name)
{

    font = TTF_OpenFont(name, 16);

    if (!font)
    {

        fprintf(stderr, "Unable to load font: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);

    }

    ofont = TTF_OpenFont(name, 16);

    if (!ofont)
    {

        fprintf(stderr, "Unable to load font: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);

    }

    TTF_SetFontOutline(ofont, 1);

}

void backend_unloadbackground(void)
{

    SDL_FreeSurface(background);

}

void backend_unloadsample(char *name)
{

    struct sample *sample = findsample(name);

    if (sample)
        Mix_FreeChunk(sample->chunk);

}

void backend_unloadfont(void)
{

    TTF_CloseFont(font);
    TTF_CloseFont(ofont);

}

void backend_init(void)
{

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {

        fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);

    }

    display = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_HWSURFACE | SDL_DOUBLEBUF);

    if (!display)
    {

        fprintf(stderr, "Unable to set video mode: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);

    }

    if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 512) < 0)
    {

        fprintf(stderr, "Unable to open audio: %s\n", Mix_GetError());
        exit(EXIT_FAILURE);

    }

    if (Mix_AllocateChannels(4) < 0)
    {

        fprintf(stderr, "Unable to allocate channels: %s\n", Mix_GetError());
        exit(EXIT_FAILURE);

    }

    if (TTF_Init() < 0)
    {

        fprintf(stderr, "Unable to init TTF: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);

    }

    SDL_ShowCursor(SDL_DISABLE);

}

void backend_destroy(void)
{

    TTF_Quit();
    Mix_CloseAudio();
    SDL_Quit();

}

