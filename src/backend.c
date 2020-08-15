#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_thread.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include "define.h"
#include "backend.h"

#define SCREEN_WIDTH                    320
#define SCREEN_HEIGHT                   240
#define SCREEN_BPP                      32

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

void backend_font_glyph(char c, unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int col)
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

void backend_paint_slider(int x, int y, int w, int h)
{

    SDL_Surface *surface = SDL_CreateRGBSurface(0, w, h, display->format->BitsPerPixel, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
    SDL_Rect r;

    r.x = x;
    r.y = y;
    r.w = w;
    r.h = h;

    dofillrectangle(surface, w, h, 0x40FFFFFF);
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

void backend_paint_menu(int x, int y, int w, int h)
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

    SDL_ShowCursor(SDL_DISABLE);

    if (TTF_Init() < 0)
        exit(EXIT_FAILURE);

}

void backend_destroy(void)
{

    TTF_Quit();
    SDL_Quit();

}

static void loadastiles(SDL_Surface *surface)
{

    SDL_Rect src;
    SDL_Rect dst;
    int x;
    int y;

    for (y = 0; y < SCREEN_HEIGHT + 36; y += surface->h)
    {

        for (x = 0; x < SCREEN_WIDTH + 36; x += surface->w)
        {

            src.x = 0;
            src.y = 0;
            src.w = surface->w;
            src.h = surface->h;
            dst.x = x;
            dst.y = y;
            dst.w = surface->w;
            dst.h = surface->h;

            SDL_BlitSurface(surface, &src, background, &dst);

        }

    }

}

void backend_loadbackground(char *name)
{

    SDL_Surface *image = IMG_Load(name);

    if (!image)
    {

        fprintf(stderr, "Unable to load image: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);

    }

    background = SDL_CreateRGBSurface(0, display->w + 36, display->h + 36, display->format->BitsPerPixel, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

    if (!background)
    {

        fprintf(stderr, "Unable to create background: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);

    }

    ssw = image->w;
    ssh = image->h;

    loadastiles(image);
    SDL_FreeSurface(image);

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

void backend_unloadfont(void)
{

    TTF_CloseFont(font);
    TTF_CloseFont(ofont);

}

void backend_play(char *name)
{

    unsigned int i;

    for (i = 0; i < nsamples; i++)
    {

        struct sample *sample = &samples[i];

        if (!strcmp(sample->name, name))
            Mix_PlayChannel(-1, sample->chunk, 0);

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

void backend_loadaudio(void)
{

    int rc;

    rc = Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 512);

    if (rc < 0)
    {

        fprintf(stderr, "Unable to open audio: %s\n", Mix_GetError());
        exit(EXIT_FAILURE);

    }

    rc = Mix_AllocateChannels(4);

    if (rc < 0)
    {

        fprintf(stderr, "Unable to allocate channels: %s\n", Mix_GetError());
        exit(EXIT_FAILURE);

    }

}

void backend_unloadaudio(void)
{

    unsigned int i;

    for (i = 0; i < nsamples; i++)
    {

        struct sample *sample = &samples[i];

        Mix_FreeChunk(sample->chunk);

    }

    Mix_CloseAudio();

}

