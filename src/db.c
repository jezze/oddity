#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "app.h"

static int attach(sqlite3 *db, char *name, char *alias)
{

    sqlite3_stmt *res;
    int rc;

    rc = sqlite3_prepare_v2(db, "ATTACH DATABASE ? AS ?", -1, &res, 0);

    if (rc != SQLITE_OK)
        return SQLITE_ERROR;

    sqlite3_bind_text(res, 1, name, -1, 0);
    sqlite3_bind_text(res, 2, alias, -1, 0);

    rc = sqlite3_step(res);

    if (rc != SQLITE_DONE)
        return SQLITE_ERROR;

    rc = sqlite3_finalize(res);

    if (rc != SQLITE_OK)
        return SQLITE_ERROR;

    return SQLITE_OK;

}

void db_loadapp(struct app *app, unsigned int id, char *name)
{

    sqlite3 *db;
    sqlite3_stmt *res;
    int rc;

    rc = sqlite3_open("db/apps.db", &db);

    if (rc != SQLITE_OK)
    {

        sqlite3_close(db);

        return;

    }

    rc = attach(db, name, "remote");

    if (rc != SQLITE_OK)
    {

        sqlite3_close(db);

        return;

    }

    rc = sqlite3_prepare_v2(db, "SELECT id, name, short FROM remote.apps WHERE id = ?", -1, &res, 0);

    if (rc != SQLITE_OK)
    {

        sqlite3_close(db);

        return;

    }

    sqlite3_bind_int(res, 1, id);

    rc = sqlite3_step(res);

    if (rc == SQLITE_ROW)
    {

        const unsigned char *name = sqlite3_column_text(res, 1);
        const unsigned char *shortdescription = sqlite3_column_text(res, 2);

        app->name = strdup((char *)name);
        app->shortdescription = strdup((char *)shortdescription);

    }

    sqlite3_finalize(res);

    sqlite3_close(db);

}

unsigned int db_countapps(char *name)
{

    sqlite3 *db;
    sqlite3_stmt *res;
    unsigned int count;
    int rc;

    rc = sqlite3_open("db/apps.db", &db);

    if (rc != SQLITE_OK)
    {

        sqlite3_close(db);

        return 0;

    }

    rc = attach(db, name, "remote");

    if (rc != SQLITE_OK)
    {

        sqlite3_close(db);

        return 0;

    }

    rc = sqlite3_prepare_v2(db, "SELECT COUNT(*) AS count FROM remote.apps", -1, &res, 0);

    if (rc != SQLITE_OK)
    {

        sqlite3_close(db);

        return 0;

    }

    rc = sqlite3_step(res);

    if (rc == SQLITE_ROW)
    {

        count = sqlite3_column_int(res, 0);

    }

    sqlite3_finalize(res);

    sqlite3_close(db);

    return count;

}

void db_loadapps(struct app *apps, unsigned int offset, unsigned int limit, char *name)
{

    sqlite3 *db;
    sqlite3_stmt *res;
    unsigned int i;
    int rc;

    rc = sqlite3_open("db/apps.db", &db);

    if (rc != SQLITE_OK)
    {

        sqlite3_close(db);

        return;

    }

    rc = attach(db, name, "remote");

    if (rc != SQLITE_OK)
    {

        sqlite3_close(db);

        return;

    }

    rc = sqlite3_prepare_v2(db, "SELECT id, name FROM remote.apps ORDER BY name LIMIT ? OFFSET ?", -1, &res, 0);

    if (rc != SQLITE_OK)
    {

        sqlite3_close(db);

        return;

    }

    sqlite3_bind_int(res, 1, limit);
    sqlite3_bind_int(res, 2, offset);

    for (i = 0; (rc = sqlite3_step(res)) == SQLITE_ROW; i++)
    {

        const unsigned char *name = sqlite3_column_text(res, 1);

        apps[i].id = sqlite3_column_int(res, 0);
        apps[i].name = strdup((char *)name);

    }

    sqlite3_finalize(res);

    sqlite3_close(db);

}

