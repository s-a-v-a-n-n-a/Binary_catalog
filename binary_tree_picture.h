#include "binary_tree_consts.h"
#include "binary_tree.h"

void tree_dot_call               (const char *name_file, const char *expansion);

void tree_print_picture_nodes    (Binary_tree *btree, long long index, FILE *picture);

void tree_generate_picture_nodes (Binary_tree *btree, long long index, FILE *picture);

void tree_print_picture_sequence (Binary_tree *btree, long long index, FILE *picture);

void tree_print_picture          (Binary_tree *btree, const char *picture_name);
