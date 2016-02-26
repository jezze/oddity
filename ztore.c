#include <stdlib.h>
#include <stdarg.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <sqlite3.h>

#define SCREEN_HEIGHT                   240
#define SCREEN_WIDTH                    320
#define SCREEN_BPP                      24
#define MENU_PADDING                    24
#define MENU_ROWS                       8
#define MENU_ROWHEIGHT                  24
#define MENUITEM_TYPE_NORMAL            0
#define MENUITEM_TYPE_BLOCKED           1
#define TEXT_XPADDING                   12
#define TEXT_YPADDING                   4

struct box
{

    unsigned int x;
    unsigned int y;
    unsigned int w;
    unsigned int h;

};

struct menuitem
{

    char *name;
    unsigned int id;
    unsigned int type;
    void (*callback)(unsigned int id);

};

struct menu
{

    unsigned int parentmenu;
    struct menuitem *items;
    unsigned int total;
    unsigned int currentitem;
    struct box box;

};

struct view
{

    void (*init)();
    void (*exit)();
    void (*render)();
    void (*handlekey)(unsigned int id);
    void (*handleevent)(unsigned int id);

};

SDL_Surface *display;
SDL_Surface *background;
SDL_Surface *blur;
TTF_Font *font;
unsigned int currentstate = 1;
unsigned int currentmenu;

struct view front;

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

struct menuitem mainmenuitems[32] = {
    {"Apps", 1, MENUITEM_TYPE_NORMAL, menuchange},
    {"Browse", 2, MENUITEM_TYPE_NORMAL, menuchange},
    {"Downloads", 0, MENUITEM_TYPE_BLOCKED, menuchange},
    {"Exit", 0, MENUITEM_TYPE_NORMAL, menuexit}
};

struct menu menus[32] = {
    {0, mainmenuitems, 4, 0, {0, 96, 320, 144}},
    {0, 0, 0, 0, {0, 0, 320, 240}},
    {0, 0, 0, 0, {0, 0, 320, 240}}
};

void menu_nextrow(struct menu *menu)
{

    menu->currentitem = (menu->total + menu->currentitem + 1) % menu->total;

}

void menu_prevrow(struct menu *menu)
{

    menu->currentitem = (menu->total + menu->currentitem - 1) % menu->total;

}

void menu_nextpage(struct menu *menu)
{

    unsigned int pagetotal = (menu->total / MENU_ROWS) + 1;
    unsigned int pageoffset = (pagetotal + (menu->currentitem / MENU_ROWS) + 1) % pagetotal;
    unsigned int rowoffset = menu->currentitem % MENU_ROWS;
    unsigned int rowstart = pageoffset * MENU_ROWS;
    unsigned int rowtotal = (menu->total - rowstart);

    menu->currentitem = rowstart + ((rowtotal <= rowoffset) ? rowtotal - 1 : rowoffset);

}

void menu_prevpage(struct menu *menu)
{

    unsigned int pagetotal = (menu->total / MENU_ROWS) + 1;
    unsigned int pageoffset = (pagetotal + (menu->currentitem / MENU_ROWS) - 1) % pagetotal;
    unsigned int rowoffset = menu->currentitem % MENU_ROWS;
    unsigned int rowstart = pageoffset * MENU_ROWS;
    unsigned int rowtotal = (menu->total - rowstart);

    menu->currentitem = rowstart + ((rowtotal <= rowoffset) ? rowtotal - 1 : rowoffset);

}

void renderbackground()
{

    SDL_BlitSurface(background, NULL, display, NULL);
    SDL_BlitSurface(blur, NULL, display, NULL);

}

void rendermenu(struct menu *menu)
{

    unsigned int page = (menu->currentitem / MENU_ROWS);
    unsigned int rowstart = page * MENU_ROWS;
    unsigned int rowend = (rowstart + MENU_ROWS) > menu->total ? menu->total : rowstart + MENU_ROWS;
    unsigned int row;
    SDL_Color color;

    for (row = rowstart; row < rowend; row++)
    {

        SDL_Surface *text;
        SDL_Rect rect;

        if (menu->items[row].type == MENUITEM_TYPE_BLOCKED)
        {

            color.r = 120;
            color.g = 120;
            color.b = 120;

        }

        else
        {

            color.r = 240;
            color.g = 240;
            color.b = 240;

        }

        text = TTF_RenderText_Solid(font, menu->items[row].name, color);
        rect.x = menu->box.x + MENU_PADDING;
        rect.y = menu->box.y + MENU_PADDING + (row - rowstart) * MENU_ROWHEIGHT;
        rect.w = menu->box.w - 2 * MENU_PADDING;
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

void loadapps(struct menu *menu, char *name)
{

    sqlite3 *db;
    sqlite3_stmt *res;
    unsigned int count;
    unsigned int i;
    int rc;

    rc = sqlite3_open(name, &db);

    if (rc != SQLITE_OK)
    {

        sqlite3_close(db);

        return;

    }

    rc = sqlite3_prepare_v2(db, "SELECT COUNT(*) FROM apps", -1, &res, 0);

    if (rc != SQLITE_OK)
    {

        sqlite3_close(db);

        return;

    }

    rc = sqlite3_step(res);

    menu->total = sqlite3_column_int(res, 0);

    sqlite3_finalize(res);

    menu->items = malloc(sizeof (struct menuitem) * menu->total);

    if (!menu->items)
    {

        sqlite3_close(db);

        return;

    }

    rc = sqlite3_prepare_v2(db, "SELECT name FROM apps ORDER BY name", -1, &res, 0);

    if (rc != SQLITE_OK)
    {

        sqlite3_close(db);

        return;

    }

    for (i = 0; (rc = sqlite3_step(res)) == SQLITE_ROW; i++)
    {

        const char *name = sqlite3_column_text(res, 0);

        menu->items[i].name = strdup(name);
        menu->items[i].id = 0;
        menu->items[i].type = MENUITEM_TYPE_NORMAL;
        menu->items[i].callback = 0;

    }

    sqlite3_finalize(res);
    sqlite3_close(db);

}

void front_init()
{

    loadapps(&menus[1], "db/apps.db");
    loadapps(&menus[2], "db/official.db");

}

void front_exit()
{

}

void front_render()
{

    renderbackground();
    rendermenu(&menus[currentmenu]);
    SDL_Flip(display);

}

void front_handlekey(unsigned int keysym)
{

    struct menu *menu = &menus[currentmenu];
    struct menuitem *menuitem = &menu->items[menu->currentitem];

    switch (keysym)
    {

    case SDLK_ESCAPE:
        currentmenu = menu->parentmenu;

        break;

    case SDLK_UP:
        menu_prevrow(menu);

        break;

    case SDLK_DOWN:
        menu_nextrow(menu);

        break;

    case SDLK_LEFT:
        menu_prevpage(menu);

        break;

    case SDLK_RIGHT:
        menu_nextpage(menu);

        break;

    case SDLK_RETURN:
        if (menuitem->callback)
            menuitem->callback(menuitem->id);

        break;

    }

}

int main(int argc, char **argv)
{

    front.init = front_init;
    front.exit = front_exit;
    front.render = front_render;
    front.handlekey = front_handlekey;

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
        exit(EXIT_FAILURE);

    if (TTF_Init() < 0)
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

    font = TTF_OpenFont("habbo.ttf", 16);

    if (!font)
        exit(EXIT_FAILURE);

    front.init();
    front.render();

    while (currentstate)
    {

        SDL_Event event;

        SDL_WaitEvent(&event);

        switch (event.type)
        {

        case SDL_KEYDOWN:
            front.handlekey(event.key.keysym.sym);

            break;

        case SDL_QUIT:
            changestate(0);

            break;

        }

        front.render();

    }

    TTF_Quit();
    SDL_Quit();

    return 0;

}

