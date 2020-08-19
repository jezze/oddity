struct selection
{

    struct list list;
    struct widget *active;

};

unsigned int selection_isactive(struct selection *selection, struct widget *widget);
void selection_move(struct selection *selection, unsigned int key);
void selection_select(struct selection *selection, unsigned int key, char *from, char *to);
void selection_unselect(struct selection *selection, unsigned int key, char *from);
void selection_add(struct selection *selection, struct widget *widget);
void selection_render(struct selection *selection, unsigned int ticks);
void selection_reset(struct selection *selection);
