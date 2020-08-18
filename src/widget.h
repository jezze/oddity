struct widget_area
{

    struct list_item item;
    struct box size;
    struct box unit;
    unsigned int selectable;

};

struct widget_slider
{

    struct list_item item;
    struct box size;
    int min;
    int max;
    int value;

};

struct widget_text
{

    struct list_item item;
    struct box size;
    unsigned int color;
    unsigned int align;
    char *data;

};

void widget_area_place(struct widget_area *area, struct box *box);
void widget_area_render(struct widget_area *area, unsigned int ticks);
void widget_area_init(struct widget_area *area, int x, int y, int w, int h);
void widget_slider_place(struct widget_slider *slider, struct box *box);
void widget_slider_render(struct widget_slider *slider, unsigned int ticks);
void widget_slider_init(struct widget_slider *slider, int min, int max, int value);
void widget_text_place(struct widget_text *text, struct box *box);
void widget_text_render(struct widget_text *text, unsigned int ticks);
void widget_text_init(struct widget_text *text, unsigned int color, unsigned int align, char *data);
