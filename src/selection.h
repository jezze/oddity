struct selection
{

    struct list list;
    struct widget *active;

};

struct widget *selection_getclosest(struct selection *selection, unsigned int key);
struct widget *selection_setclosest(struct selection *selection, unsigned int key);
unsigned int selection_isactive(struct selection *selection, struct widget *widget);
void selection_select(struct selection *selection, unsigned int key, char *from, char *to);
void selection_unselect(struct selection *selection, unsigned int key, char *from);
void selection_add(struct selection *selection, struct widget *widget);
void selection_render(struct selection *selection, unsigned int ticks);
void selection_reset(struct selection *selection);
