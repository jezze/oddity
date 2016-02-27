#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "ztore.h"
#include "app.h"
#include "text.h"
#include "menu.h"

void db_loadapp(struct app *app, unsigned int id, char *name)
{

    sqlite3 *db;
    sqlite3_stmt *res;
    int rc;

    rc = sqlite3_open(name, &db);

    if (rc != SQLITE_OK)
    {

        sqlite3_close(db);

        return;

    }

    rc = sqlite3_prepare_v2(db, "SELECT id, name, short FROM apps WHERE id = ?", -1, &res, 0);

    if (rc != SQLITE_OK)
    {

        sqlite3_close(db);

        return;

    }

    else
    {

        sqlite3_bind_int(res, 1, id);

    }

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


void db_loadapps(struct menu *menu, char *name)
{

    sqlite3 *db;
    sqlite3_stmt *res;
    unsigned int i;
    int rc;

    rc = sqlite3_open(name, &db);

    if (rc != SQLITE_OK)
    {

        sqlite3_close(db);

        return;

    }

    rc = sqlite3_prepare_v2(db, "SELECT COUNT(*) FROM apps", -1, &res, 0);

    if (rc != SQLITE_OK)
    {

        sqlite3_close(db);

        return;

    }

    rc = sqlite3_step(res);

    menu->total = sqlite3_column_int(res, 0);

    sqlite3_finalize(res);

    menu->items = malloc(sizeof (struct menuitem) * menu->total);

    if (!menu->items)
    {

        sqlite3_close(db);

        return;

    }

    rc = sqlite3_prepare_v2(db, "SELECT id, name FROM apps ORDER BY name", -1, &res, 0);

    if (rc != SQLITE_OK)
    {

        sqlite3_close(db);

        return;

    }

    for (i = 0; (rc = sqlite3_step(res)) == SQLITE_ROW; i++)
    {

        const unsigned char *name = sqlite3_column_text(res, 1);

        menu->items[i].text.content = strdup((char *)name);
        menu->items[i].id = sqlite3_column_int(res, 0);;
        menu->items[i].type = MENUITEM_FLAG_NORMAL;

    }

    sqlite3_finalize(res);
    sqlite3_close(db);

}

