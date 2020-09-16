#define WIDGET_TYPE_NONE                0
#define WIDGET_TYPE_AREA                1
#define WIDGET_TYPE_ICON                2
#define WIDGET_TYPE_OPTION              3
#define WIDGET_TYPE_SELECT              4
#define WIDGET_TYPE_SLIDER              5
#define WIDGET_TYPE_TEXT                6
#define WIDGET_TYPE_TOGGLE              7
#define WIDGET_ID_DEFAULT               ""
#define WIDGET_IN_DEFAULT               "main"

struct widget_area
{

    int x;
    int y;
    int w;
    int h;

};

struct widget_icon
{

    unsigned int type;

};

struct widget_option
{

    char *name;
    char *value;

};

struct widget_select
{

    char *current;

};

struct widget_slider
{

    int min;
    int max;
    int value;

};

struct widget_text
{

    unsigned int type;
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
    struct box size;
    unsigned int hidden;
    unsigned int selectable;

    union
    {

        struct widget_area area;
        struct widget_icon icon;
        struct widget_option option;
        struct widget_select select;
        struct widget_slider slider;
        struct widget_text text;
        struct widget_toggle toggle;

    } payload;

};

void widget_area_place(struct widget *widget, struct box *box);
void widget_area_render(struct widget *widget, unsigned int ticks);
void widget_area_set(struct widget *widget, char *key, char *value);
void widget_area_init(struct widget *widget, char *id, char *in, int x, int y, int w, int h);
void widget_icon_place(struct widget *widget, struct box *box);
void widget_icon_render(struct widget *widget, unsigned int ticks);
void widget_icon_set(struct widget *widget, char *key, char *value);
void widget_icon_init(struct widget *widget, char *id, char *in, unsigned int type);
void widget_option_place(struct widget *widget, struct box *box);
void widget_option_render(struct widget *widget, unsigned int ticks);
void widget_option_set(struct widget *widget, char *key, char *value);
void widget_option_init(struct widget *widget, char *id, char *in, char *name, char *value);
void widget_select_place(struct widget *widget, struct box *box);
void widget_select_render(struct widget *widget, unsigned int ticks);
void widget_select_set(struct widget *widget, char *key, char *value);
void widget_select_init(struct widget *widget, char *id, char *in);
void widget_slider_place(struct widget *widget, struct box *box);
void widget_slider_render(struct widget *widget, unsigned int ticks);
void widget_slider_set(struct widget *widget, char *key, char *value);
void widget_slider_init(struct widget *widget, char *id, char *in, int min, int max, int value);
void widget_text_place(struct widget *widget, struct box *box);
void widget_text_render(struct widget *widget, unsigned int ticks);
void widget_text_set(struct widget *widget, char *key, char *value);
void widget_text_init(struct widget *widget, char *id, char *in, unsigned int type, unsigned int align, char *data);
void widget_toggle_place(struct widget *widget, struct box *box);
void widget_toggle_render(struct widget *widget, unsigned int ticks);
void widget_toggle_set(struct widget *widget, char *key, char *value);
void widget_toggle_init(struct widget *widget, char *id, char *in, unsigned int state);
