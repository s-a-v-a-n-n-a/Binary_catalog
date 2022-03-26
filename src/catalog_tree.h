#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "catalog_tree_consts.h"
#include "catalog_tree_read_from_file.h"

const int    ABSENT      = -1;

const double RESIZE_UP   = 2;
const double RESIZE_DOWN = 0.5;

const char YES    = 0;
const char NO     = 1;

typedef enum tree_code_errors
{
    TREE_OK,
    TREE_NULL,
    TREE_SEG_FAULT,
    TREE_DELETED,
    TREE_NO_CONSTRUCT,
    TREE_UNDERFLOW,
    TREE_OVERFLOW,
    TREE_NO_MEMORY,
    TREE_TOO_BIG,
    TREE_CONNECT_ERROR
} tree_code;

typedef struct Catalog_tree_node Node;
struct Catalog_tree_node
{
	long long parent;
	long long left_son;
	long long right_son;

    char way;

	long long next;

	size_t length_from_beginning;
	size_t message_length;
};

typedef struct Catalog_tree_type Catalog_tree;
struct Catalog_tree_type
{
    Node     *tree;

    long long size;
    long long capacity;

    long long root_index;
    long long first_free;

    Catalog  *catalog;
};

extern const char* tree_state_text[];

typedef enum tree_functions_identifiers
{
    CONSTRUCTION,
    DESTRUCTION,
    INSERTION,
    RESIZING,
    REMOVING,
} tree_functions;

extern const char* TREE_FUNCTION_IDENTIFIERS[];

Catalog_tree    *tree_init                   (FILE *input, const long long amount);
void             tree_delete                 (Catalog_tree *ctree);
tree_code        tree_construct              (Catalog_tree *ctree, FILE *input, const long long amount);
void             tree_destruct               (Catalog_tree *ctree);

long long        tree_parent                 (Catalog_tree* ctree, long long index);
long long        tree_right_son              (Catalog_tree* ctree, long long index);
long long        tree_left_son               (Catalog_tree* ctree, long long index);
size_t           tree_message_length         (Catalog_tree* ctree, long long index);
char*            tree_message_beginnig       (Catalog_tree* ctree, long long index);

int              tree_if_lief                (Catalog_tree *ctree, long long index);

tree_code        tree_resize                 (Catalog_tree *ctree, const double coefficient);

long long        tree_search                 (Catalog_tree* ctree, const char* message);

tree_code        tree_insert                 (Catalog_tree *ctree, long long *index_after, char answer, size_t length_from_beginnig, size_t message_length);     //!!!!!!!!!!!!!

static void      tree_remove_lief            (Catalog_tree *ctree, long long index_after, char answer);     
static void      tree_remove_branch          (Catalog_tree *ctree, long long branch_base);
tree_code        tree_clean_branch           (Catalog_tree *ctree, long long branch_base);

static tree_code tree_check_pointer          (Catalog_tree *ctree);
static void      tree_check_connections      (Catalog_tree *ctree, long long index, tree_code *error);

void             tree_print_errors           (tree_code code);
void             tree_dump                   (Catalog_tree *ctree, tree_code code, const tree_functions function);
tree_code        tree_verifier               (Catalog_tree *ctree, const tree_functions requester);

void             tree_dot_call               (const char *name_file, const char *expansion);
void             tree_print_message_in_file  (Catalog_tree *ctree, long long index, FILE *picture);
void             tree_print_picture_nodes    (Catalog_tree *ctree, long long index, FILE *picture);
void             tree_generate_picture_nodes (Catalog_tree *ctree, long long index, FILE *picture);
void             tree_print_picture_sequence (Catalog_tree *ctree, long long index, FILE *picture);
void             tree_print_picture          (Catalog_tree *ctree, const char *picture_name);
