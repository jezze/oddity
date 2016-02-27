#include <stdlib.h>
#include <stdarg.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <sqlite3.h>

#define SCREEN_WIDTH                    320
#define SCREEN_HEIGHT                   240
#define SCREEN_BPP                      24
#define MENU_PADDING                    24
#define MENU_ROWHEIGHT                  24
#define MENUITEM_FLAG_NORMAL            0
#define MENUITEM_FLAG_BLOCKED           1
#define MENUITEM_FLAG_SELECTED          2
#define TEXT_XPADDING                   12
#define TEXT_YPADDING                   4

struct box
{

    unsigned int x;
    unsigned int y;
    unsigned int w;
    unsigned int h;

};

struct text
{

    char *content;

};

struct menuitem
{

    struct text text;
    unsigned int id;
    unsigned int type;

};

struct menu
{

    struct menuitem *items;
    unsigned int total;
    unsigned int currentitem;
    struct box box;

};

struct view
{

    void (*init)();
    void (*destroy)();
    void (*render)();
    void (*handlekey)(unsigned int id);
    void (*handleevent)(unsigned int id);

};

SDL_Surface *display;
SDL_Surface *background;
SDL_Surface *blur;
TTF_Font *font;
unsigned int currentstate = 1;

struct menuitem front_menuitems[32] = {
    {{"Apps"}, 1, MENUITEM_FLAG_NORMAL | MENUITEM_FLAG_SELECTED},
    {{"Browse"}, 2, MENUITEM_FLAG_NORMAL},
    {{"Downloads"}, 0, MENUITEM_FLAG_BLOCKED},
    {{"Exit"}, 8, MENUITEM_FLAG_NORMAL}
};

struct menu frontmenu = {front_menuitems, 4, 0, {0, 96, 320, 144}};
struct menu appsmenu = {0, 0, 0, {0, 0, 320, 240}};
struct menu browsemenu = {0, 0, 0, {0, 0, 320, 240}};
struct view front;
struct view apps;
struct view browse;
struct view *currentview;

void changestate(unsigned int state)
{

    currentstate = state;

}

void showview(struct view *view)
{

    if (currentview && currentview->destroy)
        currentview->destroy();

    currentview = view;

    if (currentview && currentview->init)
        currentview->init();

}

void menu_setrow(struct menu *menu, unsigned int index)
{

    menu->items[menu->currentitem].type &= ~MENUITEM_FLAG_SELECTED;
    menu->currentitem = index;
    menu->items[menu->currentitem].type |= MENUITEM_FLAG_SELECTED;

}

void menu_nextrow(struct menu *menu)
{

    menu_setrow(menu, (menu->total + menu->currentitem + 1) % menu->total);

}

void menu_prevrow(struct menu *menu)
{

    menu_setrow(menu, (menu->total + menu->currentitem - 1) % menu->total);

}

void menu_nextpage(struct menu *menu)
{

    unsigned int pagerows = (menu->box.h - 2 * MENU_PADDING) / MENU_ROWHEIGHT;
    unsigned int pagetotal = (menu->total / pagerows) + 1;
    unsigned int pageoffset = (pagetotal + (menu->currentitem / pagerows) + 1) % pagetotal;
    unsigned int rowoffset = menu->currentitem % pagerows;
    unsigned int rowstart = pageoffset * pagerows;
    unsigned int rowtotal = (menu->total - rowstart);

    menu_setrow(menu, rowstart + ((rowtotal <= rowoffset) ? rowtotal - 1 : rowoffset));

}

void menu_prevpage(struct menu *menu)
{

    unsigned int pagerows = (menu->box.h - 2 * MENU_PADDING) / MENU_ROWHEIGHT;
    unsigned int pagetotal = (menu->total / pagerows) + 1;
    unsigned int pageoffset = (pagetotal + (menu->currentitem / pagerows) - 1) % pagetotal;
    unsigned int rowoffset = menu->currentitem % pagerows;
    unsigned int rowstart = pageoffset * pagerows;
    unsigned int rowtotal = (menu->total - rowstart);

    menu_setrow(menu, rowstart + ((rowtotal <= rowoffset) ? rowtotal - 1 : rowoffset));

}

void renderbackground()
{

    SDL_BlitSurface(background, NULL, display, NULL);
    SDL_BlitSurface(blur, NULL, display, NULL);

}

void rendertext(struct text *text, SDL_Rect rect, SDL_Color color)
{

    SDL_Surface *surface = TTF_RenderText_Solid(font, text->content, color);

    rect.x = rect.x + TEXT_XPADDING;
    rect.y = rect.y + TEXT_YPADDING;
    rect.w = rect.w - TEXT_XPADDING;
    rect.h = rect.h - TEXT_YPADDING;

    SDL_BlitSurface(surface, NULL, display, &rect);
    SDL_FreeSurface(surface);

}

void rendermenuitem(struct menuitem *menuitem, SDL_Rect rect)
{

    SDL_Color color;

    if (menuitem->type & MENUITEM_FLAG_BLOCKED)
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

    if (menuitem->type & MENUITEM_FLAG_SELECTED)
        rectangleRGBA(display, rect.x, rect.y, rect.x + rect.w - 1, rect.y + rect.h - 1, 0x60, 0xC0, 0xC0, 0xFF);

    rendertext(&menuitem->text, rect, color);

}

void rendermenu(struct menu *menu)
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

        rendermenuitem(&menu->items[row], rect);

    }

}

void loadapps(struct menu *menu, char *name)
{

    sqlite3 *db;
    sqlite3_stmt *res;
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

        const unsigned char *name = sqlite3_column_text(res, 0);

        menu->items[i].text.content = strdup((char *)name);
        menu->items[i].id = 0;
        menu->items[i].type = MENUITEM_FLAG_NORMAL;

    }

    sqlite3_finalize(res);
    sqlite3_close(db);

}

void front_render()
{

    renderbackground();
    rendermenu(&frontmenu);

}

void front_handlekey(unsigned int keysym)
{

    switch (keysym)
    {

    case SDLK_UP:
        menu_prevrow(&frontmenu);

        break;

    case SDLK_DOWN:
        menu_nextrow(&frontmenu);

        break;

    case SDLK_LEFT:
        menu_prevpage(&frontmenu);

        break;

    case SDLK_RIGHT:
        menu_nextpage(&frontmenu);

        break;

    case SDLK_RETURN:
        front.handleevent(frontmenu.items[frontmenu.currentitem].id);

        break;

    }

}

void front_handleevent(unsigned int id)
{

    switch (id)
    {

    case 1:
        showview(&apps);

        break;

    case 2:
        showview(&browse);

        break;

    case 8:
        changestate(0);

        break;

    }

}

void apps_init()
{

    if (!appsmenu.items)
    {

        loadapps(&appsmenu, "db/apps.db");
        menu_setrow(&appsmenu, 0);

    }

}

void apps_render()
{

    renderbackground();
    rendermenu(&appsmenu);

}

void apps_handlekey(unsigned int keysym)
{

    switch (keysym)
    {

    case SDLK_ESCAPE:
        showview(&front);

        break;

    case SDLK_UP:
        menu_prevrow(&appsmenu);

        break;

    case SDLK_DOWN:
        menu_nextrow(&appsmenu);

        break;

    case SDLK_LEFT:
        menu_prevpage(&appsmenu);

        break;

    case SDLK_RIGHT:
        menu_nextpage(&appsmenu);

        break;

    }

}

void apps_handleevent(unsigned int id)
{

}

void browse_init()
{

    if (!browsemenu.items)
    {

        loadapps(&browsemenu, "db/official.db");
        menu_setrow(&browsemenu, 0);

    }

}

void browse_render()
{

    renderbackground();
    rendermenu(&browsemenu);

}

void browse_handlekey(unsigned int keysym)
{

    switch (keysym)
    {

    case SDLK_ESCAPE:
        showview(&front);

        break;

    case SDLK_UP:
        menu_prevrow(&browsemenu);

        break;

    case SDLK_DOWN:
        menu_nextrow(&browsemenu);

        break;

    case SDLK_LEFT:
        menu_prevpage(&browsemenu);

        break;

    case SDLK_RIGHT:
        menu_nextpage(&browsemenu);

        break;

    }

}

void browse_handleevent(unsigned int id)
{

}

int main(int argc, char **argv)
{

    front.render = front_render;
    front.handlekey = front_handlekey;
    front.handleevent = front_handleevent;
    apps.init = apps_init;
    apps.render = apps_render;
    apps.handlekey = apps_handlekey;
    apps.handleevent = apps_handleevent;
    browse.init = browse_init;
    browse.render = browse_render;
    browse.handlekey = browse_handlekey;
    browse.handleevent = browse_handleevent;

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

    showview(&front);
    currentview->render();
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
            changestate(0);

            break;

        }

        currentview->render();
        SDL_Flip(display);

    }

    TTF_Quit();
    SDL_Quit();

    return 0;

}

