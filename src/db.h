struct db_remote
{

    unsigned int id;
    char *name;

};

struct db_remotelist
{

    struct db_remote *items;
    unsigned int count;

};

struct db_app
{

    unsigned int id;
    char *name;
    char *shortdescription;

};

struct db_applist
{

    struct db_app *items;
    unsigned int count;

};

struct db_package
{

    unsigned int id;
    char *name;
    char *date;
    char *sha1;

};

struct db_packagelist
{

    struct db_package *items;
    unsigned int count;

};

void db_init();
int db_sync(struct db_remote *remote);
void db_createremote(struct db_remote *remote, unsigned int id, char *name);
void db_freeremote(struct db_remote *remote);
int db_loadremote(struct db_remote *remote, unsigned int id);
int db_loadremotes(struct db_remotelist *list);
void db_createapp(struct db_app *app, unsigned int id, char *name, char *shortdescription);
void db_freeapp(struct db_app *app);
int db_loadapp(struct db_app *app, unsigned int id);
int db_loadapps(struct db_applist *list);
int db_loadappsfromremote(struct db_applist *list, struct db_remote *remote);
void db_createpackage(struct db_package *package, unsigned int id, char *name, char *date, char *sha1);
void db_freepackage(struct db_package *package);
int db_loadpackage(struct db_package *package, unsigned int id);
int db_loadpackagesfromapp(struct db_packagelist *list, struct db_app *app);
int db_loadpackagesfromremoteapp(struct db_packagelist *list, struct db_remote *remote, struct db_app *app);
