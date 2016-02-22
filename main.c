#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

void render()
{

}

int main(int argc, char **argv)
{

    SDL_Surface *display;
    SDL_Surface *background;
    SDL_Surface *backgroundimg;
    SDL_Surface *text;
    SDL_Color xcolor;
    TTF_Font *font;
    unsigned int running = 1;
    unsigned int color;

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
        return 1;

    if (TTF_Init() < 0)
        return 1;

    display = SDL_SetVideoMode(320, 240, 32, SDL_SWSURFACE | SDL_DOUBLEBUF);

    if (!display)
        return 1;

    SDL_ShowCursor(0);

    backgroundimg = IMG_Load("back.png");

    if (!backgroundimg)
        return 1;

    font = TTF_OpenFont("habbo.ttf", 16);

    if (!font)
        return 1;

    xcolor.r = 255;
    xcolor.g = 255;
    xcolor.b = 255;

    text = TTF_RenderText_Solid(font, "Application", xcolor);

    background = SDL_CreateRGBSurface(0, 320, 240, 32, 0, 0, 0, 0);
    color = SDL_MapRGB(background->format, 255, 0, 0);

    SDL_FillRect(background, NULL, color);
    SDL_BlitSurface(background, NULL, display, NULL);
    SDL_BlitSurface(backgroundimg, NULL, display, NULL);
    SDL_BlitSurface(text, NULL, display, NULL);
    SDL_Flip(display);

    while (running)
    {

        SDL_Event event;

        while (SDL_PollEvent(&event))
        {

            switch (event.type)
            {

            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    running = 0;

                break;

            case SDL_QUIT:
                running = 0;

                break;

            }

        }
 
        render();

    }

    TTF_Quit();
    SDL_Quit();

    return 0;

}

