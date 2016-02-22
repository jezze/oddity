#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

struct menuitem
{

    char *name;
    unsigned int id;
    void (*callback)(unsigned int id);

};

struct menu
{

    char *name;
    unsigned int parentmenu;
    struct menuitem *items;
    unsigned int total;
    unsigned int currentitem;
    unsigned int x;
    unsigned int y;

};

SDL_Surface *display;
SDL_Surface *background;
SDL_Surface *blur;
TTF_Font *font;
unsigned int currentstate = 1;
unsigned int frame;
unsigned int animating;
unsigned int currentmenu;

void changestate(unsigned int state)
{

    currentstate = state;

}

void menuchange(unsigned int id)
{

    currentmenu = id;

}

void menuexit(unsigned int id)
{

    changestate(0);

}

struct menuitem mainmenuitems[3] = {
    {"Applications", 1, menuchange},
    {"Games", 2, menuchange},
    {"Exit", 0, menuexit}
};

struct menuitem applicationsitems[2] = {
    {"Deluxe Paint", 0, 0},
    {"Photoshop", 0, 0}
};

struct menuitem gamesitems[1] = {
    {"DOOM", 0, 0}
};

struct menu menus[3] = {
    {"Main Menu", 0, mainmenuitems, 3, 0, 40, 40},
    {"Applications", 0, applicationsitems, 2, 0, 40, 40},
    {"Games", 0, gamesitems, 1, 0, 40, 40}
};

void renderbackground()
{

    SDL_BlitSurface(background, NULL, display, NULL);
    SDL_BlitSurface(blur, NULL, display, NULL);

}

void rendermenu()
{

    struct menu *menu = &menus[currentmenu];
    unsigned int i;

    for (i = 0; i < menu->total; i++)
    {

        SDL_Surface *text;
        SDL_Color color;
        SDL_Rect rect;

        if (i == menu->currentitem)
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

        rect.x = menu->x;
        rect.y = menu->y + i * 20;
        rect.w = 100;
        rect.h = 100;

        text = TTF_RenderText_Solid(font, menu->items[i].name, color);

        SDL_BlitSurface(text, NULL, display, &rect);
        SDL_FreeSurface(text);

    }

}

void render()
{

    renderbackground();
    rendermenu();
    SDL_Flip(display);

    frame++;

}

void handlekeydown(SDL_Event *event)
{

    struct menu *menu = &menus[currentmenu];

    switch (event->key.keysym.sym)
    {

    case SDLK_ESCAPE:
        currentmenu = menu->parentmenu;

        break;

    case SDLK_UP:
        menu->currentitem = (menu->currentitem > 0) ? menu->currentitem - 1 : menu->total - 1;

        break;

    case SDLK_DOWN:
        menu->currentitem = (menu->currentitem < menu->total - 1) ? menu->currentitem + 1 : 0;

        break;

    case SDLK_RETURN:
        if (menu->items[menu->currentitem].callback)
            menu->items[menu->currentitem].callback(menu->items[menu->currentitem].id);

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

    while (currentstate)
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
                changestate(0);

            break;

        }

        render();

    }

    TTF_Quit();
    SDL_Quit();

    return 0;

}

