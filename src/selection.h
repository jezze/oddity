struct selection
{

    struct list list;
    struct list_item *active;

};

struct list_item *selection_getclosest(struct selection *selection, unsigned int key);
struct list_item *selection_setclosest(struct selection *selection, unsigned int key);
unsigned int selection_isactive(struct selection *selection, struct list_item *item);
void selection_select(struct selection *selection, unsigned int key, char *from, char *to);
void selection_unselect(struct selection *selection, unsigned int key, char *from);
void selection_add(struct selection *selection, struct list_item *item);
void selection_render(struct selection *selection);
void selection_reset(struct selection *selection);
