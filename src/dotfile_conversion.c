#include "../include/dotfile_conversion.h"
#include "../include/bdd_node.h"
#include "../include/unique.h"

/*---------Function Definitions---------*/

/**
 * Create a table that holds already visited BDD Nodes.
 */
struct visited **visited_create(size_t size){
    struct visited **v = calloc(sizeof(struct visited *), VISITED_TABLE_SIZE);
    for(int i = 0; i < VISITED_TABLE_SIZE; i++){
        v[i] = malloc(sizeof(struct visited));
        v[i]->node = NULL;
        v[i]->next = NULL;
    }

    return(v);
}

/**
 * Destroys the inserted visited table. And frees its memory.
 */
void visited_destroy(struct visited **v){
    for(int i = 0; i < VISITED_TABLE_SIZE; i++){
        struct visited *curr = v[i];
        while(curr != NULL){
            struct visited *next = curr->next;
            free(curr);
            curr = next;
        }
    }
    free(v);
}

size_t visited_hash(struct bdd_node *f){
    return((size_t)f % VISITED_TABLE_SIZE);
}

/**
 * If a BDD Node is already inside the visited table return true, otherwise return false.
 */
bool visited_get(struct visited **v, struct bdd_node *f){
    if(v == NULL || f == NULL)
        return(NULL);

    size_t index = visited_hash(f);
    struct visited *curr = v[index];
    while(curr != NULL){
        if(bdd_equal(curr->node, f))
            return(true);
        curr = curr->next;
    }

    return(false);
}

/**
 * Inserts a BDD Node into the visited table.
 */
void visited_add(struct visited **v, struct bdd_node *f){
    if(v == NULL || f == NULL)
        return;

    if(visited_get(v, f))
        return;

    size_t index = visited_hash(f);
    if(v[index]->node != NULL){
        struct visited *curr = v[index];
        while(curr->next != NULL){
            curr = curr->next;
        }
        curr->next = malloc(sizeof(struct visited));
        curr->next->node = f;
        curr->next->next = NULL;
    } else{
        v[index]->node = f;
        v[index]->next = NULL;
    }
}

/*------BDD to Dotfile Logic------*/

void bdd_dot(struct bdd_node *f, FILE *file, struct visited **v){
    if(file == NULL || f == NULL || v == NULL)
        return;

    if(visited_get(v, f))
        return;

    visited_add(v, f);

    if(bdd_is_terminal(f))
        fprintf(file, "\t\"%p\" [label=\"%ld\", shape=square];\n", f, f->var);
    else
        fprintf(file, "\t\"%p\" [label=\"x%ld\", shape=circle];\n", f, f->var);

    if(f->low){
        fprintf(file, "\t\"%p\" -> \"%p\" [style=dashed];\n", f, f->low);
    }
    bdd_dot(f->low, file, v);
    
    if(f->high){
        fprintf(file, "\t\"%p\" -> \"%p\";\n", f, f->high);
    }
    bdd_dot(f->high, file, v);
}

/**
 * Makes a .dot file out of a BDD.
 */
void bdd_to_dot(struct bdd_node *f, const char *file_name){
    FILE *file = fopen(file_name, "w");
    if(file == NULL)
        return;

    fprintf(file, "digraph BDD{\n");

    struct visited **v = visited_create(VISITED_TABLE_SIZE);

    bdd_dot(f, file, v);

    visited_destroy(v);

    fprintf(file, "}\n");

    fclose(file);
}