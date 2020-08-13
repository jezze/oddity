struct selection
{

    struct list list;
    struct list_item *active;

};

struct list_item *selection_getclosest(struct selection *selection, unsigned int key);
struct list_item *selection_setclosest(struct selection *selection, unsigned int key);
void selection_select(struct selection *selection, unsigned int key, struct list_item *item, char *from, char *to);
void selection_return(struct selection *selection, unsigned int key, char *from);
