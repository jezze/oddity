unsigned int view_isactive(struct view *view, char *id);
void view_moveselection(struct view *view, unsigned int key);
void view_select(struct view *view, unsigned int key, char *match, char *from, char *to);
void view_unselect(struct view *view, unsigned int key, char *from);
void view_addselection(struct view *view, struct widget *widget);
void view_renderselection(struct view *view, unsigned int ticks);
void view_reset(struct view *view);
