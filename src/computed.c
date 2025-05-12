#include "../include/computed.h"
#include "../include/bdd_node.h"

/*---------Global Definitions---------*/

struct computed **computed = NULL;

/*---------Function Definitions---------*/

size_t computed_size(){
    return(COMPUTED_TABLE_SIZE);
}

size_t computed_hash(struct bdd_node *f,struct bdd_node *g, struct bdd_node *h, unsigned short type){
    if(f == NULL)
        return(0);

    if(type == 1){
        return(((uintptr_t)f * 34) % COMPUTED_TABLE_SIZE);
    } else if(type == 2){
        if(g == NULL)
            return(0);

        return((((uintptr_t)f * 34) ^ ((uintptr_t)g * 55)) % COMPUTED_TABLE_SIZE);
    } else if(type == 3){
        if(g == NULL || h == NULL)
            return(0);

        return((((uintptr_t)f * 34) ^ ((uintptr_t)g * 55) ^ ((uintptr_t)h * 89)) % COMPUTED_TABLE_SIZE);
    }

    return(0);
}

/**
 * Creates a computed table, for already computed BDDs.
 */
struct computed **computed_create(size_t size){
    struct computed **c = calloc(sizeof(struct computed *), computed_size());
    for(int i = 0; i < computed_size(); i++){
        c[i] = malloc(sizeof(struct computed));
        c[i]->data = NULL;
        c[i]->f = NULL;
        c[i]->g = NULL;
        c[i]->h = NULL;
        c[i]->next = NULL;
    }

    return(c);
}

/**
 * Destroys the inserted computed table.
 */
void computed_destroy(struct computed **c){
    for(int i = 0; i < computed_size(); i++){
        struct computed *curr = c[i];
        while(curr != NULL){
            struct computed *next = curr->next;
            free(curr);
            curr = next;
        }
    }
    free(c);
}

/**
 * Returns the BDD with the given key triple (F,G,H) according to the given type.
 */
struct bdd_node *computed_get(struct computed **c, struct bdd_node *f, struct bdd_node *g, struct bdd_node *h, unsigned short type){
    if(c == NULL || f == NULL)
        return(NULL);

    size_t index = computed_hash(f, g, h, type);
    struct computed *curr = c[index];
    while(curr != NULL){
        if((type == 1 && bdd_equal(curr->f, f)) //Only f as key
        || (type == 2 && bdd_equal(curr->f, f) && bdd_equal(curr->g, g)) //Only f and g as key
        || (type == 3 && bdd_equal(curr->f, f) && bdd_equal(curr->g, g) && bdd_equal(curr->h, h))) //f, g and h as key
            return(curr->data);
        curr = curr->next;
    }

    return(NULL);
}

/**
 * Inserts the BDD r with the key triple (F,G,H) to the computed table c according to the inserted type.
 */
void computed_add(struct computed **c, struct bdd_node *r, struct bdd_node *f, struct bdd_node *g, struct bdd_node *h, unsigned short type){
    if(c == NULL || r == NULL || f == NULL || g == NULL || h == NULL)
        return;

    if(computed_get(c, f, g, h, type) != NULL)
        return;

    size_t index = computed_hash(f, g, h, type);
    if(c[index]->data != NULL){
        struct computed *curr = c[index];
        while(curr->next != NULL){
            curr = curr->next;
        }
        curr->next = malloc(sizeof(struct computed));
        curr->next->data = r;
        curr->next->f = f;
        curr->next->g = g;
        curr->next->h = h;
        curr->next->next = NULL;
    } else{
        c[index]->data = r;
        c[index]->f = f;
        c[index]->g = g;
        c[index]->h = h;
        c[index]->next = NULL;
    }
}
