#include "../include/aiger.h"
#include "../include/bdd_node.h"
#include "../include/unique.h"
#include "../include/dotfile_conversion.h"
#include "../include/variable_order.h"
#include <string.h>
#include <time.h>

/* Type Definitions */

struct parsed_node{
    size_t id;
    struct bdd_node *node;
};

size_t parsed_node_size = 0;

/*---------Function Definitions---------*/

struct parsed_node *parsed_node_create(size_t size){
    parsed_node_size = size;
    struct parsed_node *pn = calloc(sizeof(struct parsed_node), parsed_node_size);
    for(int i = 0; i < parsed_node_size; i++){
        pn[i].id = 0;
        pn[i].node = NULL;
    }

    return(pn);
}

void parsed_node_destroy(struct parsed_node *pn){
    if(pn == NULL)
        return;

    /*for(int i = 0; i < parsed_node_size; i++){
        free(pn[i].node);
    }*/
    free(pn);
}

size_t parsed_node_get_id(struct parsed_node *pn, size_t id){
    if(pn == NULL)
        return(SIZE_MAX);

    for(size_t i = 0; i < parsed_node_size; i++){
        if(pn[i].id == id)
            return(i);
    }

    return(0);
}

void parsed_node_add(struct parsed_node *pn, size_t index, struct bdd_node *node, size_t id){
    if(pn == NULL || node == NULL || index < 0 || id < 0)
        return;

    pn[index].id = id; 
    pn[index].node = node;
}

/**
 * 
 * Beschreibung des Formats gefunden auf: https://fmv.jku.at/aiger/FORMAT.aiger
 */
struct bdd_node **aiger_parser(const char *location, const char *file_name){
    //Create a string that holds the file name, limited to 256 characters
    char fn[256] = "";
    strcat(fn, location);
    strcat(fn, file_name);
    strcat(fn, ".aag");

    FILE *file = fopen(fn, "r");
    if(file == NULL){
        printf("Error while opening file %s.\n", fn);
        exit(1);
    }

    clock_t start = clock();


    char line[256] = "";

    //Start setting up the needed datastructs
    fgets(line, sizeof(line), file);

    char format[4];
    size_t max_var, i_size, l_size, o_size, ag_size;

    sscanf(line, "%s%zu%zu%zu%zu%zu", format, &max_var, &i_size, &l_size, &o_size, &ag_size);
    //printf("%s", line);

    struct bdd_node **f = calloc(sizeof(struct bdd_node *), o_size);

    //size_t *var_order = var_order_create(i_size);
    
    if(strcmp("aag", format) != 0){
        printf("File %s not in aag format.\n", file_name);
        exit(1);
    }

    struct parsed_node *pn = parsed_node_create((max_var)*2);

    //Insert input nodes
    for(size_t i = 0; i < i_size*2; i++){
        if(i % 2 == 0){
            fgets(line, sizeof(line), file);
            sscanf(line, "%zu", &pn[i].id);
            pn[i].node = unique_get_add(unique, (i / 2) + 1, t0, t1);
        } else{
            pn[i].id = pn[i-1].id + 1;
            pn[i].node = bdd_negate(pn[i-1].node);
        }
    }

    //Insert output
    size_t outputs[o_size];
    for(size_t i = 0; i < o_size; i++){
        fgets(line, sizeof(line), file);
        sscanf(line, "%zu", &outputs[i]);
    }

    //And-Gates
    for(int i = (i_size)*2; i < parsed_node_size; i++){
        if(i % 2 == 0){
            size_t in1, in2;
            fgets(line, sizeof(line), file);
            sscanf(line, "%zu%zu%zu", &pn[i].id, &in1, &in2);
            pn[i].node = bdd_and(pn[parsed_node_get_id(pn, in1)].node, pn[parsed_node_get_id(pn, in2)].node);
        } else{
            pn[i].id = pn[i-1].id + 1;
            pn[i].node = bdd_negate(pn[i-1].node);
        }
    }

    //f befüllen
    for(size_t i = 0; i < o_size; i++){
        f[i] = pn[parsed_node_get_id(pn, outputs[i])].node;
    }

    parsed_node_destroy(pn);

    //var_order_destroy(var_order);

    clock_t end = clock();
    double elapsed = (double)(end - start)/CLOCKS_PER_SEC;

    printf("%s.aag converted into a BDD in %.0f ms.\n", file_name, elapsed*1000);

    fclose(file);
    
    /*char out_file[256] = "output/";
    strcat(out_file, file_name);
    strcat(out_file, ".dot");
    bdd_to_dot(f[o_size-1], out_file);*/

    return(f);
}
