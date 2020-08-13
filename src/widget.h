struct widget_area
{

    struct list_item item;
    struct box size;
    struct box unit;
    unsigned int selectable;

};

struct widget_text
{

    struct list_item item;
    struct box size;
    unsigned int color;
    unsigned int align;
    char *data;

};

void widget_area_bounds(struct widget_area *area, struct box *box);
void widget_area_place(struct widget_area *area, int x, int y, int w, int h);
void widget_area_render(struct widget_area *area);
void widget_area_init(struct widget_area *area, int x, int y, int w, int h);
void widget_text_bounds(struct widget_text *text, struct box *box);
void widget_text_place(struct widget_text *text, int x, int y, int w, int h);
void widget_text_render(struct widget_text *text);
void widget_text_init(struct widget_text *text, unsigned int color, unsigned int align, char *data);
