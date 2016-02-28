struct app
{

    unsigned int id;
    char *name;
    char *shortdescription;

};

struct applist
{

    struct app *items;
    unsigned int count;

};
