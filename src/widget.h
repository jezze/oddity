#define WIDGET_TYPE_NONE                0
#define WIDGET_TYPE_AREA                1
#define WIDGET_TYPE_ICON                2
#define WIDGET_TYPE_SLIDER              3
#define WIDGET_TYPE_TEXT                4
#define WIDGET_TYPE_TOGGLE              5
#define WIDGET_ID_DEFAULT               ""
#define WIDGET_IN_DEFAULT               "main"

struct widget_area
{

    struct box unit;

};

struct widget_icon
{

    unsigned int type;

};

struct widget_slider
{

    int min;
    int max;
    int value;

};

struct widget_text
{

    unsigned int color;
    unsigned int align;
    char *data;

};

struct widget_toggle
{

    unsigned int state;

};

struct widget
{

    char *id;
    char *in;
    unsigned int type;
    struct list_item item;
    struct list_item selectionitem;
    struct box size;
    unsigned int selectable;

    union
    {

        struct widget_area area;
        struct widget_icon icon;
        struct widget_slider slider;
        struct widget_text text;
        struct widget_toggle toggle;

    } payload;

};

void widget_area_place(struct widget *widget, struct box *box);
void widget_area_render(struct widget *widget, unsigned int ticks);
void widget_area_init(struct widget *widget, char *id, char *in, int x, int y, int w, int h);
void widget_icon_place(struct widget *widget, struct box *box);
void widget_icon_render(struct widget *widget, unsigned int ticks);
void widget_icon_init(struct widget *widget, char *id, char *in, unsigned int type);
void widget_slider_place(struct widget *widget, struct box *box);
void widget_slider_render(struct widget *widget, unsigned int ticks);
void widget_slider_init(struct widget *widget, char *id, char *in, int min, int max, int value);
void widget_text_place(struct widget *widget, struct box *box);
void widget_text_render(struct widget *widget, unsigned int ticks);
void widget_text_init(struct widget *widget, char *id, char *in, unsigned int color, unsigned int align, char *data);
void widget_toggle_place(struct widget *widget, struct box *box);
void widget_toggle_render(struct widget *widget, unsigned int ticks);
void widget_toggle_init(struct widget *widget, char *id, char *in, unsigned int state);
