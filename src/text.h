#define TEXT_XPADDING                   12
#define TEXT_YPADDING                   4

struct text
{

    char *content;

};

struct textbox
{

    struct text text;
    struct box box;

};

void text_render(struct text *text, int x, int y, int w, int h, int r, int g, int b);
void text_renderbox(struct textbox *textbox, int r, int g, int b);
