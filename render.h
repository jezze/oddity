void render_image(SDL_Surface *image, SDL_Surface *display);
void render_text(struct text *text, SDL_Surface *display, SDL_Rect rect, SDL_Color color);
void render_textbox(struct textbox *textbox, SDL_Surface *display);
void render_menuitem(struct menuitem *menuitem, SDL_Surface *display, SDL_Rect rect);
void render_menu(struct menu *menu, SDL_Surface *display);
void render_initfont();
void render_destroyfont();
