#include <stdlib.h>
#include <stdarg.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <sqlite3.h>
#include "ztore.h"
#include "text.h"
#include "menu.h"
#include "render.h"

SDL_Surface *display;
SDL_Surface *background;
SDL_Surface *blur;
unsigned int currentstate = 1;
struct view views[64];
struct view *currentview;

void ztore_changestate(unsigned int state)
{

    currentstate = state;

}

void view_set(unsigned int index)
{

    if (currentview && currentview->destroy)
        currentview->destroy();

    currentview = &views[index];

    if (currentview && currentview->init)
        currentview->init();

}

void view_handleevent(unsigned int index, unsigned int id)
{

    struct view *view = &views[index];

    view->handleevent(id);

}

void view_register(unsigned int index, void (*init)(), void (*destroy)(), void (*render)(), void (*handlekey)(unsigned int id), void (*handleevent)(unsigned int id))
{

    struct view *view = &views[index];

    view->init = init;
    view->destroy = destroy;
    view->render = render;
    view->handlekey = handlekey;
    view->handleevent = handleevent;

}

int main(int argc, char **argv)
{

    view_frontsetup();
    view_appssetup();
    view_browsesetup();

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
    render_initfont();
    view_set(0);
    render_image(background, display);
    currentview->render(display);
    SDL_Flip(display);

    while (currentstate)
    {

        SDL_Event event;

        SDL_WaitEvent(&event);

        switch (event.type)
        {

        case SDL_KEYDOWN:
            currentview->handlekey(event.key.keysym.sym);

            break;

        case SDL_QUIT:
            ztore_changestate(0);

            break;

        }

        render_image(background, display);
        currentview->render(display);
        SDL_Flip(display);

    }

    render_destroyfont();
    SDL_Quit();

    return 0;

}

