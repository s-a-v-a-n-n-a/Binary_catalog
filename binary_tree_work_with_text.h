#pragma once

#include "binary_tree_consts.h"
#include "binary_tree_read_from_file.c"
#include "binary_tree_read_from_file.h"
#include "binary_tree.c"

void         tree_print_definition(Binary_tree *btree, const long long place, long long from);

void         tree_make_definition (Binary_tree *btree, const char *node);

size_t       tree_count_nodes     (File_text *base);

Binary_tree *tree_read_from_base  (File_text *base);

void         tree_make_comparison ();

void         tree_add_feature     (Binary_tree *btree, long long index, const char *new_node, const char *feature);

void         tree_add_node        (); //хз зачем пока

Binary_tree *tree_read_from_file  (const char *file_name);

void         tree_print_padding   (FILE *file_name, int padding);

void         tree_write_in_file   (Binary_tree *btree, long long index, FILE *file_name, int padding);

