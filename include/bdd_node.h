#ifndef BDD_NODE_H
#define BDD_NODE_H

#include "utils.h"

/*---------Type Declarations---------*/

struct bdd_node{
    size_t var;
    struct bdd_node *low;
    struct bdd_node *high;
};

/*---------Global Declarations---------*/

extern struct bdd_node *t0, *t1;

/*---------Function Declarations---------*/

struct bdd_node *bdd_create_node(size_t var, struct bdd_node *low, struct bdd_node *high);

void bdd_destroy_node(struct bdd_node *f);

bool bdd_is_terminal(struct bdd_node *f);

bool bdd_equal(struct bdd_node *f, struct bdd_node *g);

struct bdd_node *bdd_cofactor(size_t xi, struct bdd_node *f, bool cofactor);

size_t bdd_get_top_variable(struct bdd_node *f, struct bdd_node *g, struct bdd_node *h);

/*------Operations------*/

struct bdd_node *bdd_negate(struct bdd_node *f);

struct bdd_node *bdd_and(struct bdd_node *f, struct bdd_node *g);

struct bdd_node *bdd_nand(struct bdd_node *f, struct bdd_node *g);

struct bdd_node *bdd_or(struct bdd_node *f, struct bdd_node *g);

struct bdd_node *bdd_nor(struct bdd_node *f, struct bdd_node *g);

struct bdd_node *bdd_xor(struct bdd_node *f, struct bdd_node *g);

struct bdd_node *bdd_xnor(struct bdd_node *f, struct bdd_node *g);

/*------ITE------*/

struct bdd_node *ite_terminal_cases(struct bdd_node *f, struct bdd_node *g, struct bdd_node *h);

struct bdd_node *ite(struct bdd_node *f, struct bdd_node *g, struct bdd_node *h);

/*------KFDD------*/

struct bdd_node *kffd_xor_terminal_cases(struct bdd_node *f, struct bdd_node *g);

struct bdd_node *kffd_xor_kfdd(struct bdd_node *f, struct bdd_node *g);

struct bdd_node *bdd_to_kfdd(struct bdd_node *f);

#endif