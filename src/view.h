struct view
{

    struct view *parent;
    void (*load)(void);

};

void view_load(struct view *view, struct view *parent);
void view_quit(struct view *view);
void view_init(struct view *view, void (*load)(void));
