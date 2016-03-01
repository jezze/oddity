#define TEXT_XPADDING                   12
#define TEXT_YPADDING                   4
#define TEXT_COLOR_NORMAL               0xFFA0C0C0
#define TEXT_COLOR_TITLE                0xFFE0E0E0
#define TEXT_COLOR_SELECT               0xFFE06020
#define TEXT_COLOR_DISABLE              0xFF406060

struct text
{

    char *content;

};

struct textbox
{

    struct text text;
    struct box box;

};

void text_render(struct text *text, int x, int y, int w, int h, unsigned int color);
void text_renderbox(struct textbox *textbox, unsigned int color);
void text_init(struct text *text, char *content);
