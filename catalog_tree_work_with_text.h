#pragma once

#include "catalog_tree_consts.h"
#include "catalog_tree_read_from_file.h"
#include "catalog_tree.h"

//Функции чтения дерева из файла
size_t        tree_count_nodes       (Catalog *base);
static char  *tree_skip_space        (char *pointer, size_t *length);
static char   tree_look_for_bracket  (char **pointer, size_t *length);
Catalog_tree *tree_read_from_base    (FILE *input);
Catalog_tree *tree_read_from_file    (const char *file_name);

//Функции для записи дерева в файл
void          tree_print_padding     (FILE *file_name, int padding);
void          tree_write_in_file     (Catalog_tree *ctree, long long index, FILE *file_name, int padding);

