#ifndef AIGER_H
#define AIGER_H

/*---------Function Declarations---------*/

struct bdd_node **aiger_parser(const char *location, const char *file_name);

#endif