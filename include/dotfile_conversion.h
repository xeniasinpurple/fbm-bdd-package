#ifndef DOTFILE_CONVERSION_H
#define DOTFILE_CONVERSION_H

#include "utils.h"
#include "unique.h"

/*---------Type Declarations---------*/

struct visited{
    struct bdd_node *node;
    struct visited *next;
};

/*---------Global Declarations---------*/


/*---------Function Declarations---------*/

void bdd_dot(struct bdd_node *f, FILE *file, struct visited **v);

void bdd_to_dot(struct bdd_node *f, const char *file_name);

#endif