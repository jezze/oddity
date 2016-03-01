#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "db.h"

static int attach(sqlite3 *db, char *name, char *alias)
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

int db_loadapp(struct db_app *app, unsigned int id, char *name)
{

    sqlite3 *db;
    sqlite3_stmt *res;
    int rc;

    rc = sqlite3_open("db/local.db", &db);

    if (rc != SQLITE_OK)
        goto fail;

    rc = attach(db, name, "remote");

    if (rc != SQLITE_OK)
        goto fail;

    rc = sqlite3_prepare_v2(db, "SELECT id, name, short FROM remote.apps WHERE id = ?", -1, &res, 0);

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

int db_countapps(struct db_applist *list, char *name)
{

    sqlite3 *db;
    sqlite3_stmt *res;
    int rc;

    rc = sqlite3_open("db/local.db", &db);

    if (rc != SQLITE_OK)
        goto fail;

    rc = attach(db, name, "remote");

    if (rc != SQLITE_OK)
        goto fail;

    rc = sqlite3_prepare_v2(db, "SELECT COUNT(*) AS count FROM remote.apps", -1, &res, 0);

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

int db_loadapps(struct db_app *apps, unsigned int offset, unsigned int limit, char *name)
{

    sqlite3 *db;
    sqlite3_stmt *res;
    unsigned int i;
    int rc;

    rc = sqlite3_open("db/local.db", &db);

    if (rc != SQLITE_OK)
        goto fail;

    rc = attach(db, name, "remote");

    if (rc != SQLITE_OK)
        goto fail;

    rc = sqlite3_prepare_v2(db, "SELECT id, name, short FROM remote.apps ORDER BY name LIMIT ? OFFSET ?", -1, &res, 0);

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

int db_loadapppackages(struct db_package *packages, struct db_app *app, unsigned int offset, unsigned int limit, char *name)
{

    sqlite3 *db;
    sqlite3_stmt *res;
    unsigned int i;
    int rc;

    rc = sqlite3_open("db/local.db", &db);

    if (rc != SQLITE_OK)
        goto fail;

    rc = attach(db, name, "remote");

    if (rc != SQLITE_OK)
        goto fail;

    rc = sqlite3_prepare_v2(db, "SELECT id, name, date, sha1 FROM remote.packages WHERE apps_id = ? ORDER BY date LIMIT ? OFFSET ?", -1, &res, 0);

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

