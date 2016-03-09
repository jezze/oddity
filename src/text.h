#define TEXT_XPADDING                   12
#define TEXT_YPADDING                   4
#define TEXT_COLOR_NORMAL               0xFFA0C0C0
#define TEXT_COLOR_TITLE                0xFFE0E0E0
#define TEXT_COLOR_SELECT               0xFFE06020
#define TEXT_COLOR_DISABLE              0xFF406060
#define TEXT_ALIGN_LEFT                 0
#define TEXT_ALIGN_RIGHT                1
#define TEXT_ALIGN_CENTER               2

void text_render(struct box *box, unsigned int color, unsigned int align, char *content);
