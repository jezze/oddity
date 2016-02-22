#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

struct menuitem
{

    char *name;
    void (*callback)();

};

struct menu
{

    struct menuitem *items;
    unsigned int total;
    unsigned int current;
    unsigned int x;
    unsigned int y;

};

SDL_Surface *display;
SDL_Surface *background;
SDL_Surface *blur;
TTF_Font *font;
unsigned int running = 1;
unsigned int frame;
unsigned int animating = 0;

void quit()
{

    running = 0;

}

struct menuitem mainmenuitems[3] = {
    {"Applications", 0},
    {"Games", 0},
    {"Exit", quit}
};

struct menu mainmenu = {mainmenuitems, 3, 0, 40, 40};

void render()
{

    SDL_Surface *text;
    unsigned int i;

    SDL_BlitSurface(background, NULL, display, NULL);
    SDL_BlitSurface(blur, NULL, display, NULL);

    for (i = 0; i < mainmenu.total; i++)
    {

        SDL_Color color;
        SDL_Rect rect;

        if (i == mainmenu.current)
        {

            color.r = 240;
            color.g = 240;
            color.b = 240;

        }

        else
        {

            color.r = 160;
            color.g = 160;
            color.b = 160;

        }

        rect.x = mainmenu.x;
        rect.y = mainmenu.y + i * 20;
        rect.w = 100;
        rect.h = 100;

        text = TTF_RenderText_Solid(font, mainmenu.items[i].name, color);

        SDL_BlitSurface(text, NULL, display, &rect);
        SDL_FreeSurface(text);

    }

    SDL_Flip(display);

    frame++;

}

void handlekeydown(SDL_Event *event)
{

    switch (event->key.keysym.sym)
    {

    case SDLK_ESCAPE:
        break;

    case SDLK_UP:
        if (mainmenu.current > 0)
            mainmenu.current--;

        break;

    case SDLK_DOWN:
        if (mainmenu.current < mainmenu.total - 1)
            mainmenu.current++;

        break;

    case SDLK_RETURN:
        if (mainmenu.items[mainmenu.current].callback)
            mainmenu.items[mainmenu.current].callback();

        break;

    }

}

int main(int argc, char **argv)
{

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
        return 1;

    if (TTF_Init() < 0)
        return 1;

    display = SDL_SetVideoMode(320, 240, 24, SDL_HWSURFACE | SDL_DOUBLEBUF);

    if (!display)
        return 1;

    SDL_ShowCursor(0);

    background = IMG_Load("back.png");

    if (!background)
        return 1;

    blur = SDL_CreateRGBSurface(0, 320, 240, 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);

    if (!blur)
        return 1;

    SDL_FillRect(blur, NULL, 0xC0000000);

    font = TTF_OpenFont("habbo.ttf", 16);

    if (!font)
        return 1;

    render();

    while (running)
    {

        SDL_Event event;

        if (!animating)
            SDL_WaitEvent(&event);
        else if (!SDL_PollEvent(&event))
            continue;

        switch (event.type)
        {

            case SDL_KEYDOWN:
                handlekeydown(&event);

                break;

            case SDL_QUIT:
                quit();

            break;

        }

        render();

    }

    TTF_Quit();
    SDL_Quit();

    return 0;

}

