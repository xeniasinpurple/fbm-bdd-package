#ifndef UNIQUE_H
#define UNIQUE_H

#include "utils.h"

/*---------Type Declarations---------*/

struct unique{
    struct bdd_node *data;
    struct unique *next;
};

/*---------Global Declarations---------*/

extern struct unique **unique;

/*---------Function Declarations---------*/

size_t unique_size();

size_t unique_hash(struct bdd_node *f);

struct unique **unique_create(size_t size);

void unique_destroy(struct unique **u);

struct bdd_node *unique_get(struct unique **u, struct bdd_node *f);

void unique_add(struct unique **u, struct bdd_node *f);

struct bdd_node *unique_get_add(struct unique **u, size_t var, struct bdd_node *low, struct bdd_node *high);

#endif