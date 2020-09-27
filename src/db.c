#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "file.h"
#include "db.h"

static void attach(sqlite3 *db, char *alias, char *name)
{

    sqlite3_stmt *res;

    if (sqlite3_prepare_v2(db, "ATTACH DATABASE ? AS ?", -1, &res, 0) != SQLITE_OK)
        exit(EXIT_FAILURE);

    sqlite3_bind_text(res, 1, name, -1, 0);
    sqlite3_bind_text(res, 2, alias, -1, 0);

    if (sqlite3_step(res) != SQLITE_DONE)
        exit(EXIT_FAILURE);

    if (sqlite3_finalize(res) != SQLITE_OK)
        exit(EXIT_FAILURE);

}

static void detach(sqlite3 *db, char *alias)
{

    sqlite3_stmt *res;

    if (sqlite3_prepare_v2(db, "DETACH DATABASE ?", -1, &res, 0) != SQLITE_OK)
        exit(EXIT_FAILURE);

    sqlite3_bind_text(res, 1, alias, -1, 0);

    if (sqlite3_step(res) != SQLITE_DONE)
        exit(EXIT_FAILURE);

    if (sqlite3_finalize(res) != SQLITE_OK)
        exit(EXIT_FAILURE);

}

static void opendatabase(sqlite3 **db)
{

    char datapath[128];

    file_getlocaldatabasepath(datapath, 128);

    if (sqlite3_open(datapath, db) != SQLITE_OK)
        exit(EXIT_FAILURE);

}

static void closedatabase(sqlite3 *db)
{

    if (sqlite3_close(db) != SQLITE_OK)
        exit(EXIT_FAILURE);

}

void db_init(void)
{

    char datapath[128];

    file_getlocaldatabasepath(datapath, 128);

    if (!file_exist(datapath))
        file_copy("db/local.db", datapath);

}

int db_sync(struct db_remote *remote)
{

    sqlite3 *db;
    sqlite3_stmt *res;
    char remotedatapath[128];

    file_getlocalremotedatabasepath(remotedatapath, 128, remote->id);
    opendatabase(&db);
    attach(db, "external", remotedatapath);

    if (sqlite3_prepare_v2(db, "INSERT OR IGNORE INTO apps (id, name, short, author, homepage, description) SELECT id, name, short, author, homepage, description FROM external.apps", -1, &res, 0) != SQLITE_OK)
        exit(EXIT_FAILURE);

    if (sqlite3_step(res) != SQLITE_DONE)
        exit(EXIT_FAILURE);

    if (sqlite3_finalize(res) != SQLITE_OK)
        exit(EXIT_FAILURE);

    if (sqlite3_prepare_v2(db, "INSERT OR IGNORE INTO packages (sha1, apps_id, date, revision, version) SELECT sha1, apps_id, date, revision, version FROM external.packages", -1, &res, 0) != SQLITE_OK)
        exit(EXIT_FAILURE);

    if (sqlite3_step(res) != SQLITE_DONE)
        exit(EXIT_FAILURE);

    if (sqlite3_finalize(res) != SQLITE_OK)
        exit(EXIT_FAILURE);

    detach(db, "external");
    closedatabase(db);

    return 1;

}

void db_createremote(struct db_remote *remote, unsigned int id, char *name, char *urldatabase, char *urlpackages)
{

    remote->id = id;
    remote->name = strdup(name);
    remote->urldatabase = strdup(urldatabase);
    remote->urlpackages = strdup(urlpackages);

}

void db_freeremote(struct db_remote *remote)
{

    free(remote->name);
    free(remote->urldatabase);
    free(remote->urlpackages);

    remote->id = 0;
    remote->name = 0;
    remote->urldatabase = 0;
    remote->urlpackages = 0;

}

void db_freeremotes(struct db_remotelist *list)
{

    unsigned int i;

    for (i = 0; i < list->count; i++)
        db_freeremote(&list->items[i]);

}

int db_loadremote(struct db_remote *remote, unsigned int id)
{

    sqlite3 *db;
    sqlite3_stmt *res;

    opendatabase(&db);

    if (sqlite3_prepare_v2(db, "SELECT id, name, urldatabase, urlpackages FROM remotes WHERE id = ?", -1, &res, 0) != SQLITE_OK)
        exit(EXIT_FAILURE);

    sqlite3_bind_int(res, 1, id);

    if (sqlite3_step(res) != SQLITE_ROW)
        exit(EXIT_FAILURE);

    db_createremote(remote, sqlite3_column_int(res, 0), (char *)sqlite3_column_text(res, 1), (char *)sqlite3_column_text(res, 2), (char *)sqlite3_column_text(res, 3));

    if (sqlite3_step(res) != SQLITE_DONE)
        exit(EXIT_FAILURE);

    if (sqlite3_finalize(res) != SQLITE_OK)
        exit(EXIT_FAILURE);

    closedatabase(db);

    return 1;

}

static unsigned int countremotes(sqlite3 *db)
{

    sqlite3_stmt *res;
    unsigned int count;

    if (sqlite3_prepare_v2(db, "SELECT COUNT(*) AS count FROM remotes", -1, &res, 0) != SQLITE_OK)
        exit(EXIT_FAILURE);

    if (sqlite3_step(res) == SQLITE_ROW)
        count = sqlite3_column_int(res, 0);

    if (sqlite3_step(res) != SQLITE_DONE)
        exit(EXIT_FAILURE);

    if (sqlite3_finalize(res) != SQLITE_OK)
        exit(EXIT_FAILURE);

    return count;

}

int db_loadremotes(struct db_remotelist *list)
{

    sqlite3 *db;
    sqlite3_stmt *res;
    unsigned int i;

    opendatabase(&db);

    list->count = countremotes(db);
    list->items = malloc(sizeof (struct db_remote) * list->count);

    if (sqlite3_prepare_v2(db, "SELECT id, name, urldatabase, urlpackages FROM remotes ORDER BY name", -1, &res, 0) != SQLITE_OK)
        exit(EXIT_FAILURE);

    for (i = 0; sqlite3_step(res) == SQLITE_ROW; i++)
        db_createremote(&list->items[i], sqlite3_column_int(res, 0), (char *)sqlite3_column_text(res, 1), (char *)sqlite3_column_text(res, 2), (char *)sqlite3_column_text(res, 3));

    if (sqlite3_finalize(res) != SQLITE_OK)
        exit(EXIT_FAILURE);

    closedatabase(db);

    return 1;

}

void db_createapp(struct db_app *app, char id[6], char *name, char *shortdescription)
{

    app->id[0] = id[0];
    app->id[1] = id[1];
    app->id[2] = id[2];
    app->id[3] = id[3];
    app->id[4] = id[4];
    app->id[5] = id[5];
    app->name = strdup(name);
    app->shortdescription = strdup(shortdescription);

}

void db_freeapp(struct db_app *app)
{

    free(app->name);
    free(app->shortdescription);

    app->id[0] = 0;
    app->id[1] = 0;
    app->id[2] = 0;
    app->id[3] = 0;
    app->id[4] = 0;
    app->id[5] = 0;
    app->name = 0;
    app->shortdescription = 0;

}

void db_freeapps(struct db_applist *list)
{

    unsigned int i;

    for (i = 0; i < list->count; i++)
        db_freeapp(&list->items[i]);

}

int db_loadapp(struct db_app *app, const char id[6])
{

    sqlite3 *db;
    sqlite3_stmt *res;

    opendatabase(&db);

    if (sqlite3_prepare_v2(db, "SELECT id, name, short FROM apps WHERE id = ?", -1, &res, 0) != SQLITE_OK)
        exit(EXIT_FAILURE);

    sqlite3_bind_text(res, 1, id, 6, 0);

    if (sqlite3_step(res) != SQLITE_ROW)
        exit(EXIT_FAILURE);

    db_createapp(app, (char *)sqlite3_column_text(res, 0), (char *)sqlite3_column_text(res, 1), (char *)sqlite3_column_text(res, 2));

    if (sqlite3_step(res) != SQLITE_DONE)
        exit(EXIT_FAILURE);

    if (sqlite3_finalize(res) != SQLITE_OK)
        exit(EXIT_FAILURE);

    closedatabase(db);

    return 1;

}

static unsigned int countapps(sqlite3 *db)
{

    sqlite3_stmt *res;
    unsigned int count;

    if (sqlite3_prepare_v2(db, "SELECT COUNT(*) AS count FROM apps", -1, &res, 0) != SQLITE_OK)
        exit(EXIT_FAILURE);

    if (sqlite3_step(res) == SQLITE_ROW)
        count = sqlite3_column_int(res, 0);

    if (sqlite3_step(res) != SQLITE_DONE)
        exit(EXIT_FAILURE);

    if (sqlite3_finalize(res) != SQLITE_OK)
        exit(EXIT_FAILURE);

    return count;

}

unsigned int db_countapps(void)
{

    sqlite3 *db;
    unsigned int count;

    opendatabase(&db);

    count = countapps(db);

    closedatabase(db);

    return count;

}

int db_loadapps(struct db_applist *list)
{

    sqlite3 *db;
    sqlite3_stmt *res;
    unsigned int i;

    opendatabase(&db);

    list->count = countapps(db);
    list->items = malloc(sizeof (struct db_app) * list->count);

    if (sqlite3_prepare_v2(db, "SELECT id, name, short FROM apps ORDER BY name", -1, &res, 0) != SQLITE_OK)
        exit(EXIT_FAILURE);

    for (i = 0; sqlite3_step(res) == SQLITE_ROW; i++)
        db_createapp(&list->items[i], (char *)sqlite3_column_text(res, 0), (char *)sqlite3_column_text(res, 1), (char *)sqlite3_column_text(res, 2));

    if (sqlite3_finalize(res) != SQLITE_OK)
        exit(EXIT_FAILURE);

    closedatabase(db);

    return 1;

}

static unsigned int countappsfromremote(sqlite3 *db, struct db_remote *remote)
{

    sqlite3_stmt *res;
    unsigned int count;

    if (sqlite3_prepare_v2(db, "SELECT COUNT(*) AS count FROM apps WHERE remotes_id = ?", -1, &res, 0) != SQLITE_OK)
        exit(EXIT_FAILURE);

    sqlite3_bind_int(res, 1, remote->id);

    if (sqlite3_step(res) == SQLITE_ROW)
        count = sqlite3_column_int(res, 0);

    if (sqlite3_step(res) != SQLITE_DONE)
        exit(EXIT_FAILURE);

    if (sqlite3_finalize(res) != SQLITE_OK)
        exit(EXIT_FAILURE);

    return count;

}

int db_loadappsfromremote(struct db_applist *list, struct db_remote *remote)
{

    sqlite3 *db;
    sqlite3_stmt *res;
    unsigned int i;

    opendatabase(&db);

    list->count = countappsfromremote(db, remote);
    list->items = malloc(sizeof (struct db_app) * list->count);

    if (sqlite3_prepare_v2(db, "SELECT id, name, short FROM apps ORDER BY name WHERE remotes_id = ?", -1, &res, 0) != SQLITE_OK)
        exit(EXIT_FAILURE);

    sqlite3_bind_int(res, 1, remote->id);

    for (i = 0; sqlite3_step(res) == SQLITE_ROW; i++)
        db_createapp(&list->items[i], (char *)sqlite3_column_text(res, 0), (char *)sqlite3_column_text(res, 1), (char *)sqlite3_column_text(res, 2));

    if (sqlite3_finalize(res) != SQLITE_OK)
        exit(EXIT_FAILURE);

    closedatabase(db);

    return 1;

}

void db_createpackage(struct db_package *package, char *sha1, char *date)
{

    package->sha1 = strdup(sha1);
    package->date = strdup(date);

}

void db_freepackage(struct db_package *package)
{

    free(package->sha1);
    free(package->date);

    package->sha1 = 0;
    package->date = 0;

}

void db_freepackages(struct db_packagelist *list)
{

    unsigned int i;

    for (i = 0; i < list->count; i++)
        db_freepackage(&list->items[i]);

}

int db_loadpackage(struct db_package *package, unsigned int id)
{

    sqlite3 *db;
    sqlite3_stmt *res;

    opendatabase(&db);

    if (sqlite3_prepare_v2(db, "SELECT sha1, date FROM packages WHERE id = ?", -1, &res, 0) != SQLITE_OK)
        exit(EXIT_FAILURE);

    sqlite3_bind_int(res, 1, id);

    if (sqlite3_step(res) != SQLITE_ROW)
        exit(EXIT_FAILURE);

    db_createpackage(package, (char *)sqlite3_column_text(res, 0), (char *)sqlite3_column_text(res, 1));

    if (sqlite3_step(res) != SQLITE_DONE)
        exit(EXIT_FAILURE);

    if (sqlite3_finalize(res) != SQLITE_OK)
        exit(EXIT_FAILURE);

    closedatabase(db);

    return 1;

}

static unsigned int countpackagesfromapp(sqlite3 *db, struct db_app *app)
{

    sqlite3_stmt *res;
    unsigned int count;

    if (sqlite3_prepare_v2(db, "SELECT COUNT(*) AS count FROM packages WHERE apps_id = ?", -1, &res, 0) != SQLITE_OK)
        exit(EXIT_FAILURE);

    sqlite3_bind_text(res, 1, app->id, 6, 0);

    if (sqlite3_step(res) == SQLITE_ROW)
        count = sqlite3_column_int(res, 0);

    if (sqlite3_step(res) != SQLITE_DONE)
        exit(EXIT_FAILURE);

    if (sqlite3_finalize(res) != SQLITE_OK)
        exit(EXIT_FAILURE);

    return count;

}

int db_loadpackagesfromapp(struct db_packagelist *list, struct db_app *app)
{

    sqlite3 *db;
    sqlite3_stmt *res;
    unsigned int i;

    opendatabase(&db);

    list->count = countpackagesfromapp(db, app);
    list->items = malloc(sizeof (struct db_package) * list->count);

    if (sqlite3_prepare_v2(db, "SELECT sha1, date FROM packages WHERE apps_id = ? ORDER BY date", -1, &res, 0) != SQLITE_OK)
        exit(EXIT_FAILURE);

    sqlite3_bind_text(res, 1, app->id, 6, 0);

    for (i = 0; sqlite3_step(res) == SQLITE_ROW; i++)
        db_createpackage(&list->items[i], (char *)sqlite3_column_text(res, 0), (char *)sqlite3_column_text(res, 1));

    if (sqlite3_finalize(res) != SQLITE_OK)
        exit(EXIT_FAILURE);

    closedatabase(db);

    return 1;

}

static unsigned int countpackagesfromremoteapp(sqlite3 *db, struct db_remote *remote, struct db_app *app)
{

    sqlite3_stmt *res;
    unsigned int count;

    if (sqlite3_prepare_v2(db, "SELECT COUNT(*) AS count FROM packages WHERE remotes_id = ? AND apps_id = ?", -1, &res, 0) != SQLITE_OK)
        exit(EXIT_FAILURE);

    sqlite3_bind_int(res, 1, remote->id);
    sqlite3_bind_text(res, 2, app->id, 6, 0);

    if (sqlite3_step(res) == SQLITE_ROW)
        count = sqlite3_column_int(res, 0);

    if (sqlite3_step(res) != SQLITE_DONE)
        exit(EXIT_FAILURE);

    if (sqlite3_finalize(res) != SQLITE_OK)
        exit(EXIT_FAILURE);

    return count;

}

int db_loadpackagesfromremoteapp(struct db_packagelist *list, struct db_remote *remote, struct db_app *app)
{

    sqlite3 *db;
    sqlite3_stmt *res;
    unsigned int i;

    opendatabase(&db);

    list->count = countpackagesfromremoteapp(db, remote, app);
    list->items = malloc(sizeof (struct db_package) * list->count);

    if (sqlite3_prepare_v2(db, "SELECT sha1, date FROM packages WHERE apps_id = ? ORDER BY date", -1, &res, 0) != SQLITE_OK)
        exit(EXIT_FAILURE);

    sqlite3_bind_text(res, 1, app->id, 6, 0);

    for (i = 0; sqlite3_step(res) == SQLITE_ROW; i++)
        db_createpackage(&list->items[i], (char *)sqlite3_column_text(res, 0), (char *)sqlite3_column_text(res, 1));

    if (sqlite3_finalize(res) != SQLITE_OK)
        exit(EXIT_FAILURE);

    closedatabase(db);

    return 1;

}

