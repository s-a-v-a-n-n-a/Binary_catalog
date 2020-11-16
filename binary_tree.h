#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "binary_tree_consts.h"

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

typedef struct Binary_tree_node Node;
struct Binary_tree_node
{
	long long parent;
	long long left_son;
	long long right_son;

	long long next;

	char      message[MAX_MESSAGE_LEN];   //плохо, сделать указатель и длину хранить
};

typedef struct Binary_tree_type Binary_tree;
struct Binary_tree_type
{
    Node     *tree;

    long long size;
    long long capacity;

    long long root_index;
    long long first_free;
};

const char *tree_state_text[]
{
    "EVERYTHING IS OKAY\n",
    "TREE DOES NOT EXIST\n",
    "MEMORY ACCESS DENIED\n",
    "DEALING WITH NON-EXISTENT UNIT OR THE UNIT WAS DAMAGED\n",
    "NO MEMORY FOR CONSTRUCTION\n",
    "NOTHING TO DELETE\n",
    "WRONG ADDRESS\n",
    "MEMORY ACCESS DENIED\n",
    "TOO BIG CAPACITY IS REQUIRED\n",
    "ERROR WITH ACCESS TO ELEMENTS\n"
};

typedef enum tree_functions_identifiers
{
    CONSTRUCTION,
    DESTRUCTION,
    INSERTION,
    RESIZING,
    REMOVING,
} tree_functions;

const char *TREE_FUNCTION_IDENTIFIERS[]  =
{
    "CONSTRUCTION",
    "DESTRUCTION",
    "INSERTION",
    "RESIZING",
    "REMOVING",
};

Binary_tree     *tree_init                (const long long amount);

void             tree_delete              (Binary_tree *btree);

tree_code        tree_construct           (Binary_tree *btree, const long long amount);

void             tree_destruct            (Binary_tree *btree);

tree_code        tree_resize              (Binary_tree *btree, const double coefficient);

tree_code        tree_insert              (Binary_tree *btree, long long *index_after, char answer, const char *message);     //!!!!!!!!!!!!!

static void      tree_remove_lief         (Binary_tree *btree, long long index_after, char answer);     //!!!!!!!!!!!!!

static void      tree_remove_branch       (Binary_tree *btree, long long branch_base);

tree_code        tree_clean_branch        (Binary_tree *btree, long long branch_base);

static tree_code tree_check_pointer       (Binary_tree *btree);

static void      tree_check_connections   (Binary_tree *btree, long long index, tree_code *error);

void             tree_print_appearance    (Binary_tree *btree);

void             tree_print_errors        (tree_code code);

void             tree_dump                (Binary_tree *btree, tree_code code, const tree_functions function);

tree_code        tree_verifier            (Binary_tree *btree, const tree_functions requester);
