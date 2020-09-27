struct db_remote
{

    unsigned int id;
    char *name;
    char *urldatabase;
    char *urlpackages;

};

struct db_remotelist
{

    struct db_remote *items;
    unsigned int count;

};

struct db_app
{

    char id[6];
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

    char *sha1;
    char *date;

};

struct db_packagelist
{

    struct db_package *items;
    unsigned int count;

};

void db_init(void);
int db_sync(struct db_remote *remote);
void db_createremote(struct db_remote *remote, unsigned int id, char *name, char *urldatabase, char *urlpackages);
void db_freeremote(struct db_remote *remote);
void db_freeremotes(struct db_remotelist *list);
int db_loadremote(struct db_remote *remote, unsigned int id);
int db_loadremotes(struct db_remotelist *list);
void db_createapp(struct db_app *app, char id[6], char *name, char *shortdescription);
void db_freeapp(struct db_app *app);
void db_freeapps(struct db_applist *list);
int db_loadapp(struct db_app *app, const char id[6]);
unsigned int db_countapps(void);
int db_loadapps(struct db_applist *list);
int db_loadappsfromremote(struct db_applist *list, struct db_remote *remote);
void db_createpackage(struct db_package *package, char *sha1, char *date);
void db_freepackage(struct db_package *package);
void db_freepackages(struct db_packagelist *list);
int db_loadpackage(struct db_package *package, unsigned int id);
int db_loadpackagesfromapp(struct db_packagelist *list, struct db_app *app);
int db_loadpackagesfromremoteapp(struct db_packagelist *list, struct db_remote *remote, struct db_app *app);
