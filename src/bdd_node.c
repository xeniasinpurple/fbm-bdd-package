#include "../include/bdd_node.h"
#include "../include/unique.h"
#include "../include/computed.h"
#include "../include/dtl.h"
#include "../include/variable_order.h"

/*---------Global Definitions---------*/

struct bdd_node *t0 = NULL, *t1 = NULL;

/*---------Function Definitions---------*/

/**
 * Returns a BDD Node with the given variable, low child and high child.
 */
struct bdd_node *bdd_create_node(size_t var, struct bdd_node *low, struct bdd_node *high){
    struct bdd_node *f = malloc(sizeof(struct bdd_node));
    f->var = var;
    f->low = low;
    f->high = high;

    return(f);
}

/**
 * Destroys the inserted BDD Node and frees its memory.
 */
void bdd_destroy_node(struct bdd_node *f){
    if(f == NULL)
        return;

    bdd_destroy_node(f->low);
    bdd_destroy_node(f->high);

    free(f);
}

/**
 * Checks if a BDD Node is a terminal.
 */
bool bdd_is_terminal(struct bdd_node *f){
    return(f->low == NULL && f->high == NULL);
}

/**
 * Checks if two BDDs are equal.
 */
bool bdd_equal(struct bdd_node *f, struct bdd_node *g){
    if(f == g)
        return(true);

    if(f == NULL && g == NULL)
        return(true);

    if(f == NULL || g == NULL)
        return(false);

    //return(f->var == g->var && bdd_equal(f->low, g->low) && bdd_equal(f->high, g->high));
    return(f->var == g->var && f->low == g->low && f->high == g->high);
}

/**
 * Returns the low or high cofactor of a BDD Node.
 * cofactor = true => high cofactor; cofactor = false => low cofactor.
 */
struct bdd_node *bdd_cofactor(size_t xi, struct bdd_node *f, bool cofactor){
    if(f->low == NULL || f->high == NULL)
        return(f);

    if(f->var == xi)
        return(cofactor ? f->high : f->low);

    return(f);
}

/**
 * Returns the highest variable of the three inserted BDD Nodes.
 * The natural variable order is x1 > ... > xn, therefor x1 would be the highest.
 */
size_t bdd_get_top_variable(struct bdd_node *f, struct bdd_node *g, struct bdd_node *h){
    size_t fv, gv, hv;
    fv = f == NULL || bdd_is_terminal(f) ? SIZE_MAX : f->var;
    gv =  g == NULL || bdd_is_terminal(g) ? SIZE_MAX : g->var;
    hv = h == NULL || bdd_is_terminal(h) ? SIZE_MAX : h->var;

    /*if(var_ord != NULL){
        size_t fi, gi, hi;
        fi = var_order_get_index(var_ord, fv);
        gi = var_order_get_index(var_ord, fv);
        hi = var_order_get_index(var_ord, fv);
        size_t topi = min(fi, min(gi, hi));
        printf("%zu\n", topi);
        
        return(var_ord[topi]);
    }*/

    return(min(fv, min(gv, hv)));    
}

/*------Operations------*/

struct bdd_node *bdd_negate(struct bdd_node *f){
    if(f == NULL)
        return(f);

    if(f->low == NULL || f->high == NULL)
        return(f == t0 ? t1 : t0);

    
    return(unique_get_add(unique, f->var, bdd_negate(f->low), bdd_negate(f->high)));
}

struct bdd_node *bdd_and(struct bdd_node *f, struct bdd_node *g){
    return(ite(f, g, t0));
}

struct bdd_node *bdd_nand(struct bdd_node *f, struct bdd_node *g){
    return(ite(f, bdd_negate(g), t1));
}

struct bdd_node *bdd_or(struct bdd_node *f, struct bdd_node *g){
    return(ite(f, t1, g));
}

struct bdd_node *bdd_nor(struct bdd_node *f, struct bdd_node *g){
    return(ite(f, t0, bdd_negate(g)));
}

struct bdd_node *bdd_xor(struct bdd_node *f, struct bdd_node *g){
    return(ite(f, bdd_negate(g), g));
}

struct bdd_node *bdd_xnor(struct bdd_node *f, struct bdd_node *g){
    return(ite(f, g, bdd_negate(g)));
}

/*------ITE------*/

/**
 * Checks if the inserted BDD triple correspond to one of the given ITE terminal cases.
 * Source: Formale Beweismethoden, Lecture 5, Page 21.
 */
struct bdd_node *ite_terminal_cases(struct bdd_node *f, struct bdd_node *g, struct bdd_node *h){
    if(bdd_is_terminal(f))
        return(f->var == 0 ? unique_get_add(unique, h->var, h->low, h->high) : unique_get_add(unique, g->var, g->low, g->high));


    if(bdd_is_terminal(g) && bdd_is_terminal(h)){
        struct bdd_node *temp = g->var == 1 && h->var == 0 ? f : bdd_negate(f);
        return(unique_get_add(unique, temp->var, temp->low, temp->high));
    }

    if(bdd_equal(g,h))
        return(unique_get_add(unique, g->var, g->low, g->high));

    return(NULL);
}

/**
 * Implementation of the ITE operator.
 * Source: Formale Beweismethoden, Lecture 5, Page 22.
 */
struct bdd_node *ite(struct bdd_node *f, struct bdd_node *g, struct bdd_node *h){
    struct bdd_node *result = ite_terminal_cases(f,g,h);
    if(result != NULL)
        return(result);

    result = computed_get(computed, f, g, h, 3);
    if(result != NULL)
        return(result);

    size_t xi = bdd_get_top_variable(f,g,h);
    struct bdd_node *result_low = ite(bdd_cofactor(xi,f,false),bdd_cofactor(xi,g,false),bdd_cofactor(xi,h,false));
    struct bdd_node *result_high = ite (bdd_cofactor(xi,f,true),bdd_cofactor(xi,g,true),bdd_cofactor(xi,h,true));

    if(bdd_equal(result_low,result_high))
        return(result_high);

    result = unique_get_add(unique, xi, result_low, result_high);

    computed_add(computed, result, f, g, h, 3);

    return(result);
}

/*-----KFDD------*/

/**
 * Checks if the inserted BDD tuple correspond to one of the given KFDD XOR terminal cases.
 * If one of the inserted BDDs is the 1-Terminal, then negate the other inserted BDD.
 * Source: Formale Beweismethoden, Lecture 5, Page 39.
 */
struct bdd_node *kffd_xor_terminal_cases(struct bdd_node *f, struct bdd_node *g){
    if(f == NULL || g == NULL)
        return(NULL);

    if(f->low == NULL || f->high == NULL)
        return(f->var == 0 ? g : bdd_negate(g));

    if(g->low == NULL || g->high == NULL)
        return(g->var == 0 ? f : bdd_negate(f));

    return(NULL);
}

/**
 * Implementation of the XOR operator on KFDDs.
 * Source: Formale Beweismethoden, Lecture 5, Page 33.
 */
struct bdd_node *kffd_xor_kfdd(struct bdd_node *f, struct bdd_node *g){
    struct bdd_node *result = kffd_xor_terminal_cases(f, g);
    if(result != NULL)
        return(result);

    result = computed_get(computed, f, g, NULL, 2);
    if(result != NULL)
        return(result);

    size_t xi = bdd_get_top_variable(f, g, NULL);
    struct bdd_node *result_low = NULL, *result_high = NULL;
    enum decomposition decomp = dtl_get(dtl, xi);
    if(decomp == shannon){
        result_low = kffd_xor_kfdd(bdd_cofactor(xi, f, false), bdd_cofactor(xi, g, false));
        result_high  = kffd_xor_kfdd(bdd_cofactor(xi, f, true), bdd_cofactor(xi, g, true));
        if(bdd_equal(result_low, result_high))
            return(result_high);
    } else if(decomp == positive_davio){
        struct bdd_node *f_diff = NULL, *g_diff = NULL;
        f_diff = ite(bdd_cofactor(xi, f, true), bdd_negate(bdd_cofactor(xi, f, false)), bdd_cofactor(xi, f, false));
        g_diff = ite(bdd_cofactor(xi, g, true), bdd_negate(bdd_cofactor(xi, g, false)), bdd_cofactor(xi, g, false));

        result_low = kffd_xor_kfdd(bdd_cofactor(xi, f, false), bdd_cofactor(xi, g, false));
        result_high  = kffd_xor_kfdd(f_diff, g_diff);
        if(bdd_equal(result_high, t0))
            return(result_low);

    } else if(decomp == negative_davio){
        struct bdd_node *f_diff = NULL, *g_diff = NULL;
        f_diff = ite(bdd_cofactor(xi, f, true), bdd_negate(bdd_cofactor(xi, f, false)), bdd_cofactor(xi, f, false));
        g_diff = ite(bdd_cofactor(xi, g, true), bdd_negate(bdd_cofactor(xi, g, false)), bdd_cofactor(xi, g, false));

        result_low = kffd_xor_kfdd(bdd_cofactor(xi, f, true), bdd_cofactor(xi, g, true));
        result_high  = kffd_xor_kfdd(f_diff, g_diff);
        if(bdd_equal(result_high, t0))
            return(result_low);
    }

    result = unique_get_add(unique, xi, result_low, result_high);
    computed_add(computed, result, f, g, NULL, 2);

    return(result);
}

/**
 * Implementation of the BDD to KFDD transformation.
 * Source: Formale Beweismethoden, Lecture 5, Page 38.
 */
struct bdd_node *bdd_to_kfdd(struct bdd_node *f){
    if(bdd_is_terminal(f))
        return(unique_get_add(unique, f->var, f->low, f->high));
    
    struct bdd_node *result = computed_get(computed, f, NULL, NULL, 1);
    if(result != NULL)
        return(result);

    size_t xi = bdd_get_top_variable(f, NULL, NULL);
    struct bdd_node *result_low = NULL, *result_high = NULL;
    enum decomposition decomp = dtl_get(dtl, xi);
    if(decomp == shannon){
        result_low = bdd_to_kfdd(bdd_cofactor(xi, f, false));
        result_high = bdd_to_kfdd(bdd_cofactor(xi, f, true));
        if(bdd_equal(result_low, result_high))
            return(result_high);
    } else if(decomp == positive_davio){
        result_low = bdd_to_kfdd(bdd_cofactor(xi, f, false));
        result_high = bdd_to_kfdd(kffd_xor_kfdd(bdd_cofactor(xi, f, false), bdd_cofactor(xi, f, true)));
        //result_high = bdd_to_kfdd(ite(bdd_cofactor(xi, f, false),bdd_negate(bdd_cofactor(xi, f, true)), bdd_cofactor(xi, f, true)));
        if(bdd_equal(result_high, t0))
            return(result_low);
    } else if(decomp == negative_davio){
        result_low = bdd_to_kfdd(bdd_cofactor(xi, f, true));
        result_high = bdd_to_kfdd(kffd_xor_kfdd(bdd_cofactor(xi, f, false), bdd_cofactor(xi, f, true)));
        //result_high = bdd_to_kfdd(ite(bdd_cofactor(xi, f, false),bdd_negate(bdd_cofactor(xi, f, true)), bdd_cofactor(xi, f, true)));
        if(bdd_equal(result_high, t0))
            return(result_low);
    }

    result = unique_get_add(unique, xi, result_low, result_high);
    computed_add(computed, result, f, NULL, NULL, 1);

    return(result);
}