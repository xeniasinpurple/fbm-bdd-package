#include "../include/dtl.h"

/*---------Global Definitions---------*/

enum decomposition *dtl = NULL;

/*---------Function Definitions---------*/

/**
 * Creates a DTL.
 * Initially every decomposition in the list is of the type Shannon.
 */
enum decomposition* dtl_create(size_t size){
    enum decomposition *e = calloc(sizeof(enum decomposition), size);

    return(e);
}

/**
 * Inserts a specific decomposition at the specified index.
 */
void dtl_add(enum decomposition *e, size_t index, enum decomposition d){
    if(e == NULL)
        return;

    size_t size = sizeof(e)/sizeof(e[0]);
    if((index-1) > size){
        return;
    }

    e[index-1] = d;
}

/**
 * Gets the decomposition at the given index.
 */
enum decomposition dtl_get(enum decomposition *e, size_t index){
    return(e[index-1]);
}

/**
 * Destroys the DTL.
 */
void dtl_destroy(enum decomposition *e){
    free(e);
}