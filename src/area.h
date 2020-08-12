struct area
{

    struct list_item item;
    struct box unit;
    struct box size;
    unsigned int selectable;

};

void area_place(struct area *area, int w, int h);
void area_render(struct area *area);
void area_init(struct area *area, int x, int y, int w, int h);
