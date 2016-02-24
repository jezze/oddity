#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "tpl.h"

struct app
{

    char *name;

};

void app_store(struct app *app)
{

    struct tpl_node *node = tpl_map("A(S(s))", app);

    tpl_pack(node, 1);
    tpl_pack(node, 1);
    tpl_pack(node, 1);
    tpl_pack(node, 1);
    tpl_dump(node, TPL_FILE, "apps.zrepo");
    tpl_free(node);

}

void handle_add()
{

    struct app item;

    item.name = "demo";

    app_store(&item);

}

void handle_list()
{

    struct app item;
    struct tpl_node *node = tpl_map("A(S(s))", &item);

    tpl_load(node, TPL_FILE, "apps.zrepo");

    while (tpl_unpack(node, 1) > 0)
        fprintf(stdout, "Name: %s\n", item.name);

    tpl_free(node);

}

int main(int argc, char **argv)
{

    if (argc < 2)
    {

        fprintf(stderr, "No argument\n");

        exit(EXIT_FAILURE);

    }

    if (!strcmp(argv[1], "add"))
        handle_add();

    if (!strcmp(argv[1], "list"))
        handle_list();

    return 0;

}

