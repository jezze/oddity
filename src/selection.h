struct selection
{

    struct list list;
    struct list_item *active;

};

struct list_item *selection_getclosest(struct selection *selection, unsigned int key);
struct list_item *selection_setclosest(struct selection *selection, unsigned int key);
