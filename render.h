void render_background();
void render_text(struct text *text, int x, int y, int w, int h, int r, int g, int b);
void render_textbox(struct textbox *textbox, int r, int g, int b);
void render_menuitem(struct menuitem *menuitem, int x, int y, int w, int h);
void render_menu(struct menu *menu);
void render_flip();
void render_waitevent(struct view *view);
void render_initfont();
void render_init();
void render_destroyfont();
void render_destroy();
