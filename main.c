#include "include/utils.h"
#include "include/bdd_node.h"
#include "include/dtl.h"
#include "include/unique.h"
#include "include/computed.h"
#include "include/dotfile_conversion.h"
#include "include/aiger.h"
#include "include/variable_order.h"

int main(int argc, char **argv){
    unique = unique_create(UNIQUE_TABLE_SIZE);
    computed = computed_create(COMPUTED_TABLE_SIZE);

    t0 = unique_get_add(unique, 0, NULL, NULL);
    t1 = unique_get_add(unique, 1, NULL, NULL);

    struct bdd_node *n1, *n2, *n3, *n4;
    n1 = bdd_create_node(1, t0, t1);
    n2 = bdd_create_node(2, t0, t1);
    n3 = bdd_create_node(3, t0, t1);
    n4 = bdd_create_node(4, t0, t1);

    dtl = dtl_create(4);
    /*dtl_add(dtl, 1, positive_davio);
    dtl_add(dtl, 2, positive_davio);
    dtl_add(dtl, 3, positive_davio);
    dtl_add(dtl, 4, positive_davio);*/

    struct bdd_node *k, *k1;
    k = bdd_to_kfdd(bdd_xor(bdd_xor(n1,n2),bdd_xor(n3,n4)));

    bdd_to_dot(k, "./output/xor_x1x2x3x4.dot");

    struct bdd_node **f = aiger_parser("benchmarks/Adders/", "u_bka8");
    struct bdd_node **f1 = aiger_parser("benchmarks/Adders/", "u_cia8");
    struct bdd_node **f2 = aiger_parser("benchmarks/Adders/", "u_cla8");
    struct bdd_node **f3 = aiger_parser("benchmarks/Adders/", "u_cosa8");
    struct bdd_node **f4 = aiger_parser("benchmarks/Adders/", "u_cska8");
    struct bdd_node **f5 = aiger_parser("benchmarks/Adders/", "u_csla8");
    struct bdd_node **f6 = aiger_parser("benchmarks/Adders/", "u_hca8");
    struct bdd_node **f7 = aiger_parser("benchmarks/Adders/", "u_ka8");
    struct bdd_node **f8 = aiger_parser("benchmarks/Adders/", "u_ksa8");
    struct bdd_node **f9 = aiger_parser("benchmarks/Adders/", "u_lfa8");
    struct bdd_node **f10 = aiger_parser("benchmarks/Adders/", "u_pg_rca8");
    struct bdd_node **f11 = aiger_parser("benchmarks/Adders/", "u_rca8");
    struct bdd_node **f12 = aiger_parser("benchmarks/Adders/", "u_sa8");
    
    dtl_destroy(dtl);
    computed_destroy(computed);
    unique_destroy(unique);

    return(0);
}