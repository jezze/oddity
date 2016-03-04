#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "file.h"
#include "db.h"

static int attach(sqlite3 *db, char *alias, char *name)
{

    sqlite3_stmt *res;
    int rc;

    rc = sqlite3_prepare_v2(db, "ATTACH DATABASE ? AS ?", -1, &res, 0);

    if (rc != SQLITE_OK)
        goto fail;

    sqlite3_bind_text(res, 1, name, -1, 0);
    sqlite3_bind_text(res, 2, alias, -1, 0);

    rc = sqlite3_step(res);

    if (rc != SQLITE_DONE)
        goto fail;

    rc = sqlite3_finalize(res);

    if (rc != SQLITE_OK)
        goto fail;

fail:
    return rc;

}

static int detach(sqlite3 *db, char *alias)
{

    sqlite3_stmt *res;
    int rc;

    rc = sqlite3_prepare_v2(db, "DETACH DATABASE ?", -1, &res, 0);

    if (rc != SQLITE_OK)
        goto fail;

    sqlite3_bind_text(res, 1, alias, -1, 0);

    rc = sqlite3_step(res);

    if (rc != SQLITE_DONE)
        goto fail;

    rc = sqlite3_finalize(res);

    if (rc != SQLITE_OK)
        goto fail;

fail:
    return rc;

}

void db_init()
{

    file_copy("db/data.db", "data.db");

}

int db_sync_remote(char *name)
{

    sqlite3 *db;
    sqlite3_stmt *res;
    char datapath[256];
    char remotepath[256];
    int rc;

    file_getpath(datapath, "data.db");
    file_getpath(remotepath, name);

    rc = sqlite3_open(datapath, &db);

    if (rc != SQLITE_OK)
        goto fail;

    rc = attach(db, "external", remotepath);

    if (rc != SQLITE_OK)
        goto fail;

    rc = sqlite3_prepare_v2(db, "INSERT OR IGNORE INTO apps (remotes_id, id, name, short, icon, preview, date, author, portauthor, homepage, description, state) SELECT 1, *, 1 FROM external.apps", -1, &res, 0);

    if (rc != SQLITE_OK)
        goto fail;

    rc = sqlite3_step(res);

    if (rc != SQLITE_DONE)
        goto fail;

    rc = sqlite3_finalize(res);

    if (rc != SQLITE_OK)
        goto fail;

    rc = detach(db, "external");

    if (rc != SQLITE_OK)
        goto fail;

fail:
    sqlite3_close(db);

    return rc;

}

int db_sync()
{

    file_copy("db/remote_1.db", "remote_1.db");
    db_sync_remote("remote_1.db");

    return SQLITE_OK;

}

int db_loadapp(struct db_app *app, unsigned int id)
{

    sqlite3 *db;
    sqlite3_stmt *res;
    char datapath[256];
    int rc;

    file_getpath(datapath, "data.db");

    rc = sqlite3_open(datapath, &db);

    if (rc != SQLITE_OK)
        goto fail;

    rc = sqlite3_prepare_v2(db, "SELECT id, name, short FROM apps WHERE id = ?", -1, &res, 0);

    if (rc != SQLITE_OK)
        goto fail;

    sqlite3_bind_int(res, 1, id);

    rc = sqlite3_step(res);

    if (rc != SQLITE_ROW)
        goto fail;

    app->id = sqlite3_column_int(res, 0);
    app->name = strdup((char *)sqlite3_column_text(res, 1));
    app->shortdescription = strdup((char *)sqlite3_column_text(res, 2));

    rc = sqlite3_step(res);

    if (rc != SQLITE_DONE)
        goto fail;

    rc = sqlite3_finalize(res);

    if (rc != SQLITE_OK)
        goto fail;

fail:
    sqlite3_close(db);

    return rc;

}

int db_freeapp(struct db_app *app)
{

    free(app->name);
    free(app->shortdescription);

    return SQLITE_OK;

}

int db_countapps(struct db_applist *list)
{

    sqlite3 *db;
    sqlite3_stmt *res;
    char datapath[256];
    int rc;

    file_getpath(datapath, "data.db");

    rc = sqlite3_open(datapath, &db);

    if (rc != SQLITE_OK)
        goto fail;

    rc = sqlite3_prepare_v2(db, "SELECT COUNT(*) AS count FROM apps", -1, &res, 0);

    if (rc != SQLITE_OK)
        goto fail;

    rc = sqlite3_step(res);

    if (rc == SQLITE_ROW)
        list->count = sqlite3_column_int(res, 0);

    rc = sqlite3_step(res);

    if (rc != SQLITE_DONE)
        goto fail;

    rc = sqlite3_finalize(res);

    if (rc != SQLITE_OK)
        goto fail;

fail:
    sqlite3_close(db);

    return rc;

}

int db_loadapps(struct db_app *apps, unsigned int offset, unsigned int limit)
{

    sqlite3 *db;
    sqlite3_stmt *res;
    char datapath[256];
    unsigned int i;
    int rc;

    file_getpath(datapath, "data.db");

    rc = sqlite3_open(datapath, &db);

    if (rc != SQLITE_OK)
        goto fail;

    rc = sqlite3_prepare_v2(db, "SELECT id, name, short FROM apps ORDER BY name LIMIT ? OFFSET ?", -1, &res, 0);

    if (rc != SQLITE_OK)
        goto fail;

    sqlite3_bind_int(res, 1, limit);
    sqlite3_bind_int(res, 2, offset);

    for (i = 0; (rc = sqlite3_step(res)) == SQLITE_ROW; i++)
    {

        apps[i].id = sqlite3_column_int(res, 0);
        apps[i].name = strdup((char *)sqlite3_column_text(res, 1));
        apps[i].shortdescription = strdup((char *)sqlite3_column_text(res, 2));

    }

    if (rc != SQLITE_DONE)
        goto fail;

    rc = sqlite3_finalize(res);

    if (rc != SQLITE_OK)
        goto fail;

fail:
    sqlite3_close(db);

    return rc;

}

int db_loadapppackages(struct db_package *packages, struct db_app *app, unsigned int offset, unsigned int limit)
{

    sqlite3 *db;
    sqlite3_stmt *res;
    char datapath[256];
    unsigned int i;
    int rc;

    file_getpath(datapath, "data.db");

    rc = sqlite3_open(datapath, &db);

    if (rc != SQLITE_OK)
        goto fail;

    rc = sqlite3_prepare_v2(db, "SELECT id, name, date, sha1 FROM packages WHERE apps_id = ? ORDER BY date LIMIT ? OFFSET ?", -1, &res, 0);

    if (rc != SQLITE_OK)
        goto fail;

    sqlite3_bind_int(res, 1, app->id);
    sqlite3_bind_int(res, 2, limit);
    sqlite3_bind_int(res, 3, offset);

    for (i = 0; (rc = sqlite3_step(res)) == SQLITE_ROW; i++)
    {

        packages[i].id = sqlite3_column_int(res, 0);
        packages[i].name = strdup((char *)sqlite3_column_text(res, 1));
        packages[i].date = strdup((char *)sqlite3_column_text(res, 2));
        packages[i].sha1 = strdup((char *)sqlite3_column_text(res, 3));

    }

    if (rc != SQLITE_DONE)
        goto fail;

    rc = sqlite3_finalize(res);

    if (rc != SQLITE_OK)
        goto fail;

fail:
    sqlite3_close(db);

    return rc;

}

