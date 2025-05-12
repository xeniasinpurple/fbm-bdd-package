#include "../include/variable_order.h"

/*size_t *var_ord = NULL;
size_t var_order_size = 0;
size_t var_order_index = 0;

size_t *var_order_create(size_t size){
    size_t *var_order = calloc(sizeof(size_t), size);
    var_order_index = 0;
    var_order_size = size;

    return(var_order);
}

void var_order_destroy(size_t *var_order){
    if(var_order == NULL)
        return;

    var_order_index = 0;
    free(var_order);
}

void var_order_add(size_t *var_order, size_t var){
    if(var_order == NULL || var >= var_order_size)
        return;

    var_order[var_order_index++] = var;
}

size_t var_order_get_index(size_t *var_order, size_t var){
    if(var_order == NULL || var >= var_order_size)
        return(SIZE_MAX);

    for(int i = 0; i < var_order_size; i++){
        if(var_order[i] == var){
            return(i);
        }
    }

    return(SIZE_MAX);
}*/