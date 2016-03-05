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

    char datapath[64];

    file_getdatabasepath(datapath, 64);

    if (sqlite3_open(datapath, db) != SQLITE_OK)
        exit(EXIT_FAILURE);

}

static void closedatabase(sqlite3 *db)
{

    if (sqlite3_close(db) != SQLITE_OK)
        exit(EXIT_FAILURE);

}

void db_init()
{

    char datapath[64];

    file_getdatabasepath(datapath, 64);

    if (!file_exist(datapath))
        file_copy("db/data.db", datapath);

}

int db_sync(struct db_remote *remote)
{

    sqlite3 *db;
    sqlite3_stmt *res;
    char remotedatapath[64];

    file_getremotedatabasepath(remotedatapath, 64, remote->id);
    opendatabase(&db);

    attach(db, "external", remotedatapath);

    if (sqlite3_prepare_v2(db, "INSERT OR IGNORE INTO apps (remotes_id, id, name, short, icon, preview, date, author, portauthor, homepage, description, state) SELECT ?, *, 1 FROM external.apps", -1, &res, 0) != SQLITE_OK)
        exit(EXIT_FAILURE);

    sqlite3_bind_int(res, 1, remote->id);

    if (sqlite3_step(res) != SQLITE_DONE)
        exit(EXIT_FAILURE);

    if (sqlite3_finalize(res) != SQLITE_OK)
        exit(EXIT_FAILURE);

    if (sqlite3_prepare_v2(db, "INSERT OR IGNORE INTO packages (remotes_id, id, apps_id, name, date, sha1, state) SELECT ?, *, 1 FROM external.packages", -1, &res, 0) != SQLITE_OK)
        exit(EXIT_FAILURE);

    sqlite3_bind_int(res, 1, remote->id);

    if (sqlite3_step(res) != SQLITE_DONE)
        exit(EXIT_FAILURE);

    if (sqlite3_finalize(res) != SQLITE_OK)
        exit(EXIT_FAILURE);

    detach(db, "external");
    closedatabase(db);

    return 1;

}

void db_createremote(struct db_remote *remote, unsigned int id, char *name, char *url)
{

    remote->id = id;
    remote->name = strdup(name);
    remote->url = strdup(url);

}

void db_freeremote(struct db_remote *remote)
{

    free(remote->name);
    free(remote->url);

    remote->id = 0;
    remote->name = 0;
    remote->url = 0;

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

    if (sqlite3_prepare_v2(db, "SELECT id, name, url FROM remotes WHERE id = ?", -1, &res, 0) != SQLITE_OK)
        exit(EXIT_FAILURE);

    sqlite3_bind_int(res, 1, id);

    if (sqlite3_step(res) != SQLITE_ROW)
        exit(EXIT_FAILURE);

    db_createremote(remote, sqlite3_column_int(res, 0), (char *)sqlite3_column_text(res, 1), (char *)sqlite3_column_text(res, 2));

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

    if (sqlite3_prepare_v2(db, "SELECT id, name, url FROM remotes ORDER BY name", -1, &res, 0) != SQLITE_OK)
        exit(EXIT_FAILURE);

    for (i = 0; sqlite3_step(res) == SQLITE_ROW; i++)
        db_createremote(&list->items[i], sqlite3_column_int(res, 0), (char *)sqlite3_column_text(res, 1), (char *)sqlite3_column_text(res, 2));

    if (sqlite3_finalize(res) != SQLITE_OK)
        exit(EXIT_FAILURE);

    closedatabase(db);

    return 1;

}

void db_createapp(struct db_app *app, unsigned int id, char *name, char *shortdescription, unsigned int state)
{

    app->id = id;
    app->name = strdup(name);
    app->shortdescription = strdup(shortdescription);
    app->state = state;

}

void db_freeapp(struct db_app *app)
{

    free(app->name);
    free(app->shortdescription);

    app->id = 0;
    app->name = 0;
    app->shortdescription = 0;
    app->state = 0;

}

void db_freeapps(struct db_applist *list)
{

    unsigned int i;

    for (i = 0; i < list->count; i++)
        db_freeapp(&list->items[i]);

}

int db_loadapp(struct db_app *app, unsigned int id)
{

    sqlite3 *db;
    sqlite3_stmt *res;

    opendatabase(&db);

    if (sqlite3_prepare_v2(db, "SELECT id, name, short, state FROM apps WHERE id = ?", -1, &res, 0) != SQLITE_OK)
        exit(EXIT_FAILURE);

    sqlite3_bind_int(res, 1, id);

    if (sqlite3_step(res) != SQLITE_ROW)
        exit(EXIT_FAILURE);

    db_createapp(app, sqlite3_column_int(res, 0), (char *)sqlite3_column_text(res, 1), (char *)sqlite3_column_text(res, 2), sqlite3_column_int(res, 3));

    if (sqlite3_step(res) != SQLITE_DONE)
        exit(EXIT_FAILURE);

    if (sqlite3_finalize(res) != SQLITE_OK)
        exit(EXIT_FAILURE);

    closedatabase(db);

    return 1;

}

int db_saveappstate(struct db_app *app)
{

    sqlite3 *db;
    sqlite3_stmt *res;

    opendatabase(&db);

    if (sqlite3_prepare_v2(db, "UPDATE apps SET state = ? WHERE id = ?", -1, &res, 0) != SQLITE_OK)
        exit(EXIT_FAILURE);

    sqlite3_bind_int(res, 1, app->state);
    sqlite3_bind_int(res, 2, app->id);

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

int db_loadapps(struct db_applist *list)
{

    sqlite3 *db;
    sqlite3_stmt *res;
    unsigned int i;

    opendatabase(&db);

    list->count = countapps(db);
    list->items = malloc(sizeof (struct db_app) * list->count);

    if (sqlite3_prepare_v2(db, "SELECT id, name, short, state FROM apps ORDER BY name", -1, &res, 0) != SQLITE_OK)
        exit(EXIT_FAILURE);

    for (i = 0; sqlite3_step(res) == SQLITE_ROW; i++)
        db_createapp(&list->items[i], sqlite3_column_int(res, 0), (char *)sqlite3_column_text(res, 1), (char *)sqlite3_column_text(res, 2), sqlite3_column_int(res, 3));

    if (sqlite3_finalize(res) != SQLITE_OK)
        exit(EXIT_FAILURE);

    closedatabase(db);

    return 1;

}

static unsigned int countinstalledapps(sqlite3 *db)
{

    sqlite3_stmt *res;
    unsigned int count;

    if (sqlite3_prepare_v2(db, "SELECT COUNT(*) AS count FROM apps WHERE state = 3", -1, &res, 0) != SQLITE_OK)
        exit(EXIT_FAILURE);

    if (sqlite3_step(res) == SQLITE_ROW)
        count = sqlite3_column_int(res, 0);

    if (sqlite3_step(res) != SQLITE_DONE)
        exit(EXIT_FAILURE);

    if (sqlite3_finalize(res) != SQLITE_OK)
        exit(EXIT_FAILURE);

    return count;

}

int db_loadinstalledapps(struct db_applist *list)
{

    sqlite3 *db;
    sqlite3_stmt *res;
    unsigned int i;

    opendatabase(&db);

    list->count = countinstalledapps(db);
    list->items = malloc(sizeof (struct db_app) * list->count);

    if (sqlite3_prepare_v2(db, "SELECT id, name, short, state FROM apps WHERE state = 3 ORDER BY name", -1, &res, 0) != SQLITE_OK)
        exit(EXIT_FAILURE);

    for (i = 0; sqlite3_step(res) == SQLITE_ROW; i++)
        db_createapp(&list->items[i], sqlite3_column_int(res, 0), (char *)sqlite3_column_text(res, 1), (char *)sqlite3_column_text(res, 2), sqlite3_column_int(res, 3));

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

    if (sqlite3_prepare_v2(db, "SELECT id, name, short, state FROM apps ORDER BY name WHERE remotes_id = ?", -1, &res, 0) != SQLITE_OK)
        exit(EXIT_FAILURE);

    sqlite3_bind_int(res, 1, remote->id);

    for (i = 0; sqlite3_step(res) == SQLITE_ROW; i++)
        db_createapp(&list->items[i], sqlite3_column_int(res, 0), (char *)sqlite3_column_text(res, 1), (char *)sqlite3_column_text(res, 2), sqlite3_column_int(res, 3));

    if (sqlite3_finalize(res) != SQLITE_OK)
        exit(EXIT_FAILURE);

    closedatabase(db);

    return 1;

}

void db_createpackage(struct db_package *package, unsigned int id, char *name, char *date, char *sha1, unsigned int state)
{

    package->id = id;
    package->name = strdup(name);
    package->date = strdup(date);
    package->sha1 = strdup(sha1);
    package->state = state;

}

void db_freepackage(struct db_package *package)
{

    free(package->name);
    free(package->date);
    free(package->sha1);

    package->id = 0;
    package->name = 0;
    package->date = 0;
    package->sha1 = 0;
    package->state = 0;

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

    if (sqlite3_prepare_v2(db, "SELECT id, name, date, sha1, state FROM packages WHERE id = ?", -1, &res, 0) != SQLITE_OK)
        exit(EXIT_FAILURE);

    sqlite3_bind_int(res, 1, id);

    if (sqlite3_step(res) != SQLITE_ROW)
        exit(EXIT_FAILURE);

    db_createpackage(package, sqlite3_column_int(res, 0), (char *)sqlite3_column_text(res, 1), (char *)sqlite3_column_text(res, 2), (char *)sqlite3_column_text(res, 3), sqlite3_column_int(res, 4));

    if (sqlite3_step(res) != SQLITE_DONE)
        exit(EXIT_FAILURE);

    if (sqlite3_finalize(res) != SQLITE_OK)
        exit(EXIT_FAILURE);

    closedatabase(db);

    return 1;

}

int db_savepackagestate(struct db_package *package)
{

    sqlite3 *db;
    sqlite3_stmt *res;

    opendatabase(&db);

    if (sqlite3_prepare_v2(db, "UPDATE packages SET state = ? WHERE id = ?", -1, &res, 0) != SQLITE_OK)
        exit(EXIT_FAILURE);

    sqlite3_bind_int(res, 1, package->state);
    sqlite3_bind_int(res, 2, package->id);

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

    sqlite3_bind_int(res, 1, app->id);

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

    if (sqlite3_prepare_v2(db, "SELECT id, name, date, sha1, state FROM packages WHERE apps_id = ? ORDER BY date", -1, &res, 0) != SQLITE_OK)
        exit(EXIT_FAILURE);

    sqlite3_bind_int(res, 1, app->id);

    for (i = 0; sqlite3_step(res) == SQLITE_ROW; i++)
        db_createpackage(&list->items[i], sqlite3_column_int(res, 0), (char *)sqlite3_column_text(res, 1), (char *)sqlite3_column_text(res, 2), (char *)sqlite3_column_text(res, 3), sqlite3_column_int(res, 4));

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
    sqlite3_bind_int(res, 2, app->id);

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

    if (sqlite3_prepare_v2(db, "SELECT id, name, date, sha1, state FROM packages WHERE remotes_id = ? AND apps_id = ? ORDER BY date", -1, &res, 0) != SQLITE_OK)
        exit(EXIT_FAILURE);

    sqlite3_bind_int(res, 1, remote->id);
    sqlite3_bind_int(res, 2, app->id);

    for (i = 0; sqlite3_step(res) == SQLITE_ROW; i++)
        db_createpackage(&list->items[i], sqlite3_column_int(res, 0), (char *)sqlite3_column_text(res, 1), (char *)sqlite3_column_text(res, 2), (char *)sqlite3_column_text(res, 3), sqlite3_column_int(res, 4));

    if (sqlite3_finalize(res) != SQLITE_OK)
        exit(EXIT_FAILURE);

    closedatabase(db);

    return 1;

}

