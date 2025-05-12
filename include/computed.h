#ifndef COMPUTED_H
#define COMPUTED_H

#include "utils.h"

/*---------Type Declarations---------*/

struct computed{
    struct bdd_node *data;
    struct bdd_node *f, *g, *h; //Key values
    struct computed *next;
};

/*---------Global Declarations---------*/

extern struct computed **computed;

/*---------Function Declarations---------*/

size_t computed_size();

size_t computed_hash(struct bdd_node *f,struct bdd_node *g, struct bdd_node *h, unsigned short type);

struct computed **computed_create(size_t size);

void computed_destroy(struct computed **c);

struct bdd_node *computed_get(struct computed **c, struct bdd_node *f, struct bdd_node *g, struct bdd_node *h, unsigned short type);

void computed_add(struct computed **c, struct bdd_node *r, struct bdd_node *f, struct bdd_node *g, struct bdd_node *h, unsigned short type);

#endif