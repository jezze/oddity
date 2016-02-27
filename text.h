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
