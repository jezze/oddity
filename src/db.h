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

int db_loadapp(struct db_app *app, unsigned int id, char *name);
int db_countapps(struct db_applist *list, char *name);
int db_loadapps(struct db_app *apps, unsigned int offset, unsigned int limit, char *name);
int db_loadapppackages(struct db_package *packages, struct db_app *app, unsigned int offset, unsigned int limit, char *name);
