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
int db_sync_remote(char *name);
int db_sync();
int db_loadapp(struct db_app *app, unsigned int id);
int db_freeapp(struct db_app *app);
int db_countapps(struct db_applist *list);
int db_loadapps(struct db_app *apps, unsigned int offset, unsigned int limit);
int db_loadapppackages(struct db_package *packages, struct db_app *app, unsigned int offset, unsigned int limit);
