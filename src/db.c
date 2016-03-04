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

static void syncremote(sqlite3 *db, char *remotedatapath, unsigned int remoteid)
{

    sqlite3_stmt *res;

    attach(db, "external", remotedatapath);

    if (sqlite3_prepare_v2(db, "INSERT OR IGNORE INTO apps (remotes_id, id, name, short, icon, preview, date, author, portauthor, homepage, description, state) SELECT ?, *, 1 FROM external.apps", -1, &res, 0) != SQLITE_OK)
        exit(EXIT_FAILURE);

    sqlite3_bind_int(res, 1, remoteid);

    if (sqlite3_step(res) != SQLITE_DONE)
        exit(EXIT_FAILURE);

    if (sqlite3_finalize(res) != SQLITE_OK)
        exit(EXIT_FAILURE);

    detach(db, "external");

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

int db_sync()
{

    sqlite3 *db;
    unsigned int i;

    opendatabase(&db);

    for (i = 0; i < 1; i++)
    {

        char remotedatapath[64];

        file_downloadremote(i + 1);
        file_getremotedatabasepath(remotedatapath, 64, i + 1);
        syncremote(db, remotedatapath, i + 1);

    }

    closedatabase(db);

    return 1;

}

void db_createremote(struct db_remote *remote, unsigned int id, char *name)
{

    remote->id = id;
    remote->name = strdup(name);

}

void db_freeremote(struct db_remote *remote)
{

    free(remote->name);

    remote->id = 0;
    remote->name = 0;

}

int db_loadremote(struct db_remote *remote, unsigned int id)
{

    sqlite3 *db;
    sqlite3_stmt *res;

    opendatabase(&db);

    if (sqlite3_prepare_v2(db, "SELECT id, name FROM remotes WHERE id = ?", -1, &res, 0) != SQLITE_OK)
        exit(EXIT_FAILURE);

    sqlite3_bind_int(res, 1, id);

    if (sqlite3_step(res) != SQLITE_ROW)
        exit(EXIT_FAILURE);

    db_createremote(remote, sqlite3_column_int(res, 0), (char *)sqlite3_column_text(res, 1));

    if (sqlite3_step(res) != SQLITE_DONE)
        exit(EXIT_FAILURE);

    if (sqlite3_finalize(res) != SQLITE_OK)
        exit(EXIT_FAILURE);

    closedatabase(db);

    return 1;

}

unsigned int db_countremotes()
{

    sqlite3 *db;
    sqlite3_stmt *res;
    unsigned int count;

    opendatabase(&db);

    if (sqlite3_prepare_v2(db, "SELECT COUNT(*) AS count FROM remotes", -1, &res, 0) != SQLITE_OK)
        exit(EXIT_FAILURE);

    if (sqlite3_step(res) == SQLITE_ROW)
        count = sqlite3_column_int(res, 0);

    if (sqlite3_step(res) != SQLITE_DONE)
        exit(EXIT_FAILURE);

    if (sqlite3_finalize(res) != SQLITE_OK)
        exit(EXIT_FAILURE);

    closedatabase(db);

    return count;

}

int db_loadremotes(struct db_remote *remotes, unsigned int offset, unsigned int limit)
{

    sqlite3 *db;
    sqlite3_stmt *res;
    unsigned int i;

    opendatabase(&db);

    if (sqlite3_prepare_v2(db, "SELECT id, name FROM remotes ORDER BY name LIMIT ? OFFSET ?", -1, &res, 0) != SQLITE_OK)
        exit(EXIT_FAILURE);

    sqlite3_bind_int(res, 1, limit);
    sqlite3_bind_int(res, 2, offset);

    for (i = 0; sqlite3_step(res) == SQLITE_ROW; i++)
        db_createremote(&remotes[i], sqlite3_column_int(res, 0), (char *)sqlite3_column_text(res, 1));

    if (sqlite3_finalize(res) != SQLITE_OK)
        exit(EXIT_FAILURE);

    closedatabase(db);

    return 1;

}

void db_createapp(struct db_app *app, unsigned int id, char *name, char *shortdescription)
{

    app->id = id;
    app->name = strdup(name);
    app->shortdescription = strdup(shortdescription);

}

void db_freeapp(struct db_app *app)
{

    free(app->name);
    free(app->shortdescription);

    app->id = 0;
    app->name = 0;
    app->shortdescription = 0;

}

int db_loadapp(struct db_app *app, unsigned int id)
{

    sqlite3 *db;
    sqlite3_stmt *res;

    opendatabase(&db);

    if (sqlite3_prepare_v2(db, "SELECT id, name, short FROM apps WHERE id = ?", -1, &res, 0) != SQLITE_OK)
        exit(EXIT_FAILURE);

    sqlite3_bind_int(res, 1, id);

    if (sqlite3_step(res) != SQLITE_ROW)
        exit(EXIT_FAILURE);

    db_createapp(app, sqlite3_column_int(res, 0), (char *)sqlite3_column_text(res, 1), (char *)sqlite3_column_text(res, 2));

    if (sqlite3_step(res) != SQLITE_DONE)
        exit(EXIT_FAILURE);

    if (sqlite3_finalize(res) != SQLITE_OK)
        exit(EXIT_FAILURE);

    closedatabase(db);

    return 1;

}

unsigned int db_countapps()
{

    sqlite3 *db;
    sqlite3_stmt *res;
    unsigned int count;

    opendatabase(&db);

    if (sqlite3_prepare_v2(db, "SELECT COUNT(*) AS count FROM apps", -1, &res, 0) != SQLITE_OK)
        exit(EXIT_FAILURE);

    if (sqlite3_step(res) == SQLITE_ROW)
        count = sqlite3_column_int(res, 0);

    if (sqlite3_step(res) != SQLITE_DONE)
        exit(EXIT_FAILURE);

    if (sqlite3_finalize(res) != SQLITE_OK)
        exit(EXIT_FAILURE);

    closedatabase(db);

    return count;

}

int db_loadapps(struct db_app *apps, unsigned int offset, unsigned int limit)
{

    sqlite3 *db;
    sqlite3_stmt *res;
    unsigned int i;

    opendatabase(&db);

    if (sqlite3_prepare_v2(db, "SELECT id, name, short FROM apps ORDER BY name LIMIT ? OFFSET ?", -1, &res, 0) != SQLITE_OK)
        exit(EXIT_FAILURE);

    sqlite3_bind_int(res, 1, limit);
    sqlite3_bind_int(res, 2, offset);

    for (i = 0; sqlite3_step(res) == SQLITE_ROW; i++)
        db_createapp(&apps[i], sqlite3_column_int(res, 0), (char *)sqlite3_column_text(res, 1), (char *)sqlite3_column_text(res, 2));

    if (sqlite3_finalize(res) != SQLITE_OK)
        exit(EXIT_FAILURE);

    closedatabase(db);

    return 1;

}

void db_createpackage(struct db_package *package, unsigned int id, char *name, char *date, char *sha1)
{

    package->id = id;
    package->name = strdup(name);
    package->date = strdup(date);
    package->sha1 = strdup(sha1);

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

}

int db_loadpackage(struct db_package *package, unsigned int id)
{

    sqlite3 *db;
    sqlite3_stmt *res;

    opendatabase(&db);

    if (sqlite3_prepare_v2(db, "SELECT id, name, date, sha1 FROM packages WHERE id = ?", -1, &res, 0) != SQLITE_OK)
        exit(EXIT_FAILURE);

    sqlite3_bind_int(res, 1, id);

    if (sqlite3_step(res) != SQLITE_ROW)
        exit(EXIT_FAILURE);

    db_createpackage(package, sqlite3_column_int(res, 0), (char *)sqlite3_column_text(res, 1), (char *)sqlite3_column_text(res, 2), (char *)sqlite3_column_text(res, 3));

    if (sqlite3_step(res) != SQLITE_DONE)
        exit(EXIT_FAILURE);

    if (sqlite3_finalize(res) != SQLITE_OK)
        exit(EXIT_FAILURE);

    closedatabase(db);

    return 1;

}

unsigned int db_countapppackages(struct db_app *app)
{

    sqlite3 *db;
    sqlite3_stmt *res;
    unsigned int count;

    opendatabase(&db);

    if (sqlite3_prepare_v2(db, "SELECT COUNT(*) AS count FROM packages WHERE apps_id = ?", -1, &res, 0) != SQLITE_OK)
        exit(EXIT_FAILURE);

    sqlite3_bind_int(res, 1, app->id);

    if (sqlite3_step(res) == SQLITE_ROW)
        count = sqlite3_column_int(res, 0);

    if (sqlite3_step(res) != SQLITE_DONE)
        exit(EXIT_FAILURE);

    if (sqlite3_finalize(res) != SQLITE_OK)
        exit(EXIT_FAILURE);

    closedatabase(db);

    return count;

}

int db_loadapppackages(struct db_package *packages, struct db_app *app, unsigned int offset, unsigned int limit)
{

    sqlite3 *db;
    sqlite3_stmt *res;
    unsigned int i;

    opendatabase(&db);

    if (sqlite3_prepare_v2(db, "SELECT id, name, date, sha1 FROM packages WHERE apps_id = ? ORDER BY date LIMIT ? OFFSET ?", -1, &res, 0) != SQLITE_OK)
        exit(EXIT_FAILURE);

    sqlite3_bind_int(res, 1, app->id);
    sqlite3_bind_int(res, 2, limit);
    sqlite3_bind_int(res, 3, offset);

    for (i = 0; sqlite3_step(res) == SQLITE_ROW; i++)
        db_createpackage(&packages[i], sqlite3_column_int(res, 0), (char *)sqlite3_column_text(res, 1), (char *)sqlite3_column_text(res, 2), (char *)sqlite3_column_text(res, 3));

    if (sqlite3_finalize(res) != SQLITE_OK)
        exit(EXIT_FAILURE);

    closedatabase(db);

    return 1;

}

