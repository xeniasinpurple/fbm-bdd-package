#include "../include/unique.h"
#include "../include/bdd_node.h"

/*---------Global Definitions---------*/

struct unique **unique = NULL;

/*---------Function Definitions---------*/

size_t unique_size(){
    return(UNIQUE_TABLE_SIZE);
}

size_t unique_hash(struct bdd_node *f){
    if(f == NULL)
        return(0);

    return(((f->var * 21) ^ ((uintptr_t)f->low * 13) ^ ((uintptr_t)f->high * 8)) % UNIQUE_TABLE_SIZE);
}

/**
 * Creates a unique table.
 */
struct unique **unique_create(size_t size){
    struct unique **u = calloc(sizeof(struct unique *), unique_size());
    for(int i = 0; i < unique_size(); i++){
        u[i] = malloc(sizeof(struct unique));
        u[i]->data = NULL;
        u[i]->next = NULL;
    }

    return(u);
}

/**
 * Destroys the inserted unique table.
 */
void unique_destroy(struct unique **u){
    for(int i = 0; i < unique_size(); i++){
        struct unique *curr = u[i];
        while(curr != NULL){
            struct unique *next = curr->next;
            free(curr);
            curr = next;
        }
    }
    free(u);
}

/**
 * Returns the inserted BDD from the unique table.
 */
struct bdd_node *unique_get(struct unique **u, struct bdd_node *f){
    if(u == NULL || f == NULL)
        return(NULL);

    size_t index = unique_hash(f);
    struct unique *curr = unique[index];
    while(curr != NULL){
        if(bdd_equal(curr->data, f))
            return(curr->data);
        curr = curr->next;
    }

    return(NULL);
}

/**
 * Inserts the given BDD into the unique table.
 */
void unique_add(struct unique **u, struct bdd_node *f){
    if(u == NULL || f == NULL)
        return;

    if(unique_get(u, f) != NULL)
        return;
    
    size_t index = unique_hash(f);
    if(u[index]->data != NULL){
        struct unique *curr = u[index];
        while(curr->next != NULL){
            curr = curr->next;
        }
        curr->next = malloc(sizeof(struct unique));
        curr->next->data = f;
        curr->next->next = NULL;
    } else{
        u[index]->data = f;
        u[index]->next = NULL;
    }
}

/**
 * If a BDD Node with the given variable, low child and high child already exists in the unique table, return it.
 * If not insert it into the unique table.
 */
struct bdd_node *unique_get_add(struct unique **u, size_t var, struct bdd_node *low, struct bdd_node *high){
    struct bdd_node *f = bdd_create_node(var, low, high);
    struct bdd_node *g = unique_get(u, f);
    if(g != NULL){
        free(f);
        return(g);
    }

    unique_add(u, f);

    return(f);
}