#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

struct app
{

    char name[64];

};

void app_init(struct app *app, char *name)
{

    strncpy(app->name, name, 64);

}

void handle_add(int argc, char **argv)
{

    FILE *db;
    struct app item;

    if (argc < 3)
        return;

    db = fopen("apps.zrepo", "ab");

    if (!db)
        return;

    app_init(&item, argv[2]);
    fwrite(&item, sizeof (struct app), 1, db);
    fclose(db);

}

void handle_list(int argc, char **argv)
{

    FILE *db;
    struct app items[8];
    unsigned int count;

    db = fopen("apps.zrepo", "rb");

    if (!db)
        return;

    while (count = fread(items, sizeof (struct app), 8, db))
    {

        unsigned int i;

        for (i = 0; i < count; i++)
            fprintf(stdout, "Name: %s\n", items[i].name);

    }

    fclose(db);

}

int main(int argc, char **argv)
{

    if (argc < 2)
    {

        fprintf(stderr, "No argument\n");

        exit(EXIT_FAILURE);

    }

    if (!strcmp(argv[1], "add"))
        handle_add(argc, argv);

    if (!strcmp(argv[1], "list"))
        handle_list(argc, argv);

    return 0;

}

