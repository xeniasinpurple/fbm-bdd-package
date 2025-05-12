#ifndef DTL_H
#define DTL_H

#include "utils.h"

/*---------Type Declarations---------*/

enum decomposition{shannon, positive_davio, negative_davio};

/*---------Global Declarations---------*/

extern enum decomposition *dtl;

/*---------Function Declarations---------*/

enum decomposition* dtl_create(size_t size);

void dtl_add(enum decomposition *e, size_t index, enum decomposition d);

enum decomposition dtl_get(enum decomposition *e, size_t index);

void dtl_destroy(enum decomposition *e);

#endif