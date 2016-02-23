#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_gfxPrimitives.h>

#define SCREEN_HEIGHT                   240
#define SCREEN_WIDTH                    320
#define SCREEN_BPP                      24
#define MENU_PADDING                    24
#define MENU_ROWS                       8
#define MENU_ROWHEIGHT                  24
#define TEXT_XPADDING                   16
#define TEXT_YPADDING                   4

struct app
{

    char *name;

};

struct app apps[3] = {
    {"Deluxe Paint"},
    {"Photoshop"},
    {"DOOM"}
};

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
    unsigned int w;
    unsigned int h;

};

SDL_Surface *display;
SDL_Surface *background;
SDL_Surface *blur;
TTF_Font *font;
unsigned int currentstate = 1;
unsigned int frame;
unsigned int animating;
unsigned int currentmenu;
unsigned int currentoverlay;
unsigned int currentapp;

void changestate(unsigned int state)
{

    currentstate = state;

}

void menuchange(unsigned int id)
{

    currentmenu = id;

}

void menuapp(unsigned int id)
{

    currentoverlay = 1;
    currentapp = id;

}

void menuexit(unsigned int id)
{

    changestate(0);

}

struct menuitem mainmenuitems[32] = {
    {"Apps", 1, menuchange},
    {"Store", 2, menuchange},
    {"Exit", 0, menuexit}
};

struct menuitem appsitems[32] = {
    {"Deluxe paint", 0, menuapp},
    {"Photoshop", 1, menuapp},
    {"DOOM", 2, menuapp}
};

struct menuitem storeitems[32] = {
    {"Deluxe paint", 0, 0},
    {"Photoshop", 1, 0},
    {"Deluxe paint", 0, 0},
    {"Photoshop", 1, 0},
    {"Deluxe paint", 0, 0},
    {"Photoshop", 1, 0},
    {"Deluxe paint", 0, 0},
    {"Photoshop", 1, 0},
    {"Deluxe paint", 0, 0},
    {"Photoshop", 1, 0},
    {"Deluxe paint", 0, 0},
    {"DOOM", 2, 0}
};

struct menu menus[32] = {
    {"Main Menu", 0, mainmenuitems, 3, 0, 0, 120, 320, 120},
    {"Apps", 0, appsitems, 3, 0, 0, 0, 320, 240},
    {"Store", 0, storeitems, 12, 0, 0, 0, 320, 240}
};

void renderbackground()
{

    SDL_BlitSurface(background, NULL, display, NULL);
    SDL_BlitSurface(blur, NULL, display, NULL);

}

void rendermenu()
{

    struct menu *menu = &menus[currentmenu];
    unsigned int rowstart = (menu->currentitem / MENU_ROWS) * MENU_ROWS;
    unsigned int rowend = (rowstart + MENU_ROWS) > menu->total ? menu->total : rowstart + MENU_ROWS;
    unsigned int row;
    SDL_Color color;

    color.r = 240;
    color.g = 240;
    color.b = 240;

    for (row = rowstart; row < rowend; row++)
    {

        SDL_Surface *text = TTF_RenderText_Solid(font, menu->items[row].name, color);
        SDL_Rect rect;

        rect.x = menu->x + MENU_PADDING;
        rect.y = menu->y + MENU_PADDING + (row - rowstart) * MENU_ROWHEIGHT;
        rect.w = menu->w - 2 * MENU_PADDING;
        rect.h = MENU_ROWHEIGHT;

        if (row == menu->currentitem)
            rectangleRGBA(display, rect.x, rect.y, rect.x + rect.w - 1, rect.y + rect.h - 1, 0x60, 0xC0, 0xC0, 0xFF);

        rect.x = rect.x + TEXT_XPADDING;
        rect.y = rect.y + TEXT_YPADDING;
        rect.w = rect.w - TEXT_XPADDING;
        rect.h = rect.h - TEXT_YPADDING;

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

    display = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_HWSURFACE | SDL_DOUBLEBUF);

    if (!display)
        return 1;

    SDL_ShowCursor(0);

    background = IMG_Load("back.png");

    if (!background)
        return 1;

    blur = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);

    if (!blur)
        return 1;

    SDL_FillRect(blur, NULL, SDL_MapRGBA(blur->format, 0x00, 0x00, 0x00, 0xE0));

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

