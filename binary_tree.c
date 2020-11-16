#pragma once

#include "binary_tree.h"
#include "binary_tree_consts.h"
#include "binary_tree_picture.c"

#define ASSERTION(code)                                                  \
    fprintf(stderr, "-----------------!WARNING!----------------\n");     \
    fprintf(stderr, "IN FILE %s\nIN LINE %d\n", __FILE__, __LINE__);     \
    tree_print_errors(code);


Binary_tree *tree_init(const long long amount)
{
    Binary_tree *btree = (Binary_tree*)calloc(1, sizeof(Binary_tree));
    if (!btree)
    {
        ASSERTION(TREE_NO_MEMORY);
        tree_dump(btree, TREE_NO_MEMORY, CONSTRUCTION);
        return NULL;
    }

    if (tree_construct(btree, amount) != TREE_OK)
    {
        tree_destruct(btree);
        return NULL;
    }

	return btree;
}

void tree_delete(Binary_tree *btree)
{
    tree_destruct(btree);

    if (btree)
        free(btree);
}

tree_code tree_construct(Binary_tree *btree, const long long amount)
{
    btree->tree     = (Node*)calloc(amount, sizeof(Node));
    if (!btree->tree)
    {
        ASSERTION(TREE_NO_MEMORY);
        tree_dump(btree, TREE_NO_MEMORY, CONSTRUCTION);
        return TREE_NO_MEMORY;
    }
    btree->capacity = amount;
    btree->size     = 0;

	btree->tree[0].parent    = ABSENT;
	btree->tree[0].right_son = ABSENT;
	btree->tree[0].left_son  = ABSENT;

    btree->root_index = 0;
	btree->first_free = 0;

	for (long long i = 0; i < amount; i++)
	{
        btree->tree[i].next = i + 1;
	}

	return TREE_OK;
}

void tree_destruct(Binary_tree *btree)
{
    if (btree->tree)
        free(btree->tree);
}

tree_code tree_resize(Binary_tree *btree, const double coefficient)
{
    size_t new_capacity = btree->capacity * coefficient;

    Node *new_tree = (Node*)realloc(btree->tree, sizeof(Node) * new_capacity);
    if (!new_tree)
    {
        ASSERTION(TREE_NO_MEMORY);
        tree_dump(btree, TREE_NO_MEMORY, RESIZING);
        return TREE_NO_MEMORY;
    }

    btree->tree = new_tree;

    if (coefficient > 1)
    {
        for (long long i = btree->capacity; i < new_capacity; i++)
        {
            btree->tree[i].next = i + 1;
        }
    }

    btree->capacity = new_capacity;

    return TREE_OK;
}

long long tree_search(Binary_tree *btree, const char *message)   //Проход по циклу почему такой?
{
    for (long long i = 0; i < btree->size; i++)
    {
        if (!strcmp(btree->tree[i].message, message))
            return i;
    }

    return ABSENT;
}

tree_code tree_insert(Binary_tree *btree, long long *index_after, char answer, const char *message)
{
	if (*index_after > btree->size + 1)
    {
        ASSERTION(TREE_OVERFLOW);
        tree_dump(btree, TREE_OVERFLOW, INSERTION);
        return TREE_OVERFLOW;
    }

    if (btree->size + 1 >= btree->capacity)
    {
        tree_code error = tree_resize(btree, RESIZE_UP);
        if (error != TREE_OK)
        {
            ASSERTION(error);
            tree_dump(btree, error, INSERTION);
            return error;
        }
    }

	if (*index_after == ABSENT)
	{
		memcpy(btree->tree[btree->root_index].message, message, MAX_MESSAGE_LEN);
		//btree->tree[btree->root_index].message = message;

		btree->tree[btree->root_index].parent    = ABSENT;
        btree->tree[btree->root_index].right_son = ABSENT;
        btree->tree[btree->root_index].left_son  = ABSENT;

        *index_after      = btree->root_index;
        btree->first_free = btree->tree[btree->root_index].next;
	}
	else
	{
		memcpy(btree->tree[btree->first_free].message, message, MAX_MESSAGE_LEN);
		//btree->tree[btree->first_free].message   = message;
		//printf("%s\n", btree->tree[btree->first_free].message);
        btree->tree[btree->first_free].parent    = *index_after;
        btree->tree[btree->first_free].right_son = ABSENT;
        btree->tree[btree->first_free].left_son  = ABSENT;

        if (answer == YES)
            btree->tree[*index_after].right_son = btree->first_free;
        else
            btree->tree[*index_after].left_son = btree->first_free;

        *index_after      = btree->first_free;
        btree->first_free = btree->tree[btree->first_free].next;
    }

    btree->size++;
}

static void tree_remove_lief(Binary_tree *btree, long long index_after, char answer)
{
    if (index_after == ABSENT)
	{
        btree->tree[btree->root_index].next = btree->first_free;
        btree->first_free = btree->root_index;
	}
	else
	{
		btree->tree[index_after].next = btree->first_free;
        btree->first_free = index_after;

        if (answer == YES)
            btree->tree[index_after].right_son = ABSENT;
        else
            btree->tree[index_after].left_son = ABSENT;
	}

	btree->size--;
}

static void tree_remove_branch(Binary_tree *btree, long long branch_base)
{
    if (btree->tree[branch_base].right_son == ABSENT && btree->tree[branch_base].left_son == ABSENT && btree->tree[branch_base].parent >= 0)
    {
        if (btree->tree[btree->tree[branch_base].parent].right_son == branch_base)
        {
            tree_remove_lief(btree, btree->tree[branch_base].parent, YES);
            return;
        }
        else
        {
            tree_remove_lief(btree, btree->tree[branch_base].parent, NO);
            return;
        }
    }

    if (btree->tree[branch_base].right_son != ABSENT)
        tree_remove_branch(btree, btree->tree[branch_base].right_son);
    if (btree->tree[branch_base].left_son != ABSENT)
        tree_remove_branch(btree, btree->tree[branch_base].left_son);

    if (btree->tree[branch_base].parent == ABSENT)
    {
        tree_remove_lief(btree, ABSENT, YES);
        return;
    }
    else
    {
        if (btree->tree[btree->tree[branch_base].parent].right_son == branch_base)
        {
            tree_remove_lief(btree, btree->tree[branch_base].parent, YES);
            return;
        }
        else
        {
            tree_remove_lief(btree, btree->tree[branch_base].parent, NO);
            return;
        }
    }
}

tree_code tree_clean_branch(Binary_tree *btree, long long branch_base)
{
    if (branch_base > btree->size || branch_base < ABSENT)
    {
        ASSERTION(TREE_UNDERFLOW);
        tree_dump(btree, TREE_UNDERFLOW, REMOVING);
        return TREE_UNDERFLOW;
    }
    tree_code error = TREE_OK;

    tree_remove_branch(btree, branch_base);

    return error;
}

tree_code tree_check_pointer(Binary_tree *btree)
{
    if (!btree || !btree->tree)
        return TREE_NULL;

    if ((size_t)(btree) <= 4096 || (size_t)(btree->tree) <= 4096)
        return TREE_SEG_FAULT;

    return TREE_OK;
}

void tree_check_connections(Binary_tree *btree, long long index, tree_code *error)
{
    if (btree->tree[index].right_son == ABSENT && btree->tree[index].left_son == ABSENT)
        return;
    else if (btree->tree[index].right_son < ABSENT || btree->tree[index].left_son < ABSENT)
    {
        *error = TREE_CONNECT_ERROR;
        return;
    }

    if (btree->tree[index].right_son != ABSENT)
    {
        if (index != btree->tree[btree->tree[index].right_son].parent)
        {
            *error = TREE_CONNECT_ERROR;
            return;
        }

        tree_check_connections(btree, btree->tree[index].right_son, error);
    }
    if (btree->tree[index].left_son != ABSENT)
    {
        if (index != btree->tree[btree->tree[index].left_son].parent)
        {
            *error = TREE_CONNECT_ERROR;
            return;
        }

        tree_check_connections(btree, btree->tree[index].left_son, error);
    }
}

void tree_print_errors(tree_code code)
{
    printf("Error: %s\n", tree_state_text[code]);
}

void tree_dump(Binary_tree *btree, tree_code code, const tree_functions function)
{
    static long int doing = 0;

    const char* mode = "";
    if (!doing)
        mode = "wb";
    else
        mode = "ab";
    FILE* log = fopen("tree_appearance.html", mode);

    fprintf(log, "<pre><font size=\"5\"  face=\"Times New Roman\">\n");

    fprintf(log, "<p><span style=\"font-weight: bold\">CURRENT STATE OF TREE</span></p>\n");
    fprintf(log, "THE NEWS FROM %s\n", TREE_FUNCTION_IDENTIFIERS[function]);

    fprintf(log, "%s", tree_state_text[code]);

    fprintf(log, "CURRENT CAPACITY IS %lld\n", btree->capacity);
    fprintf(log, "CURRENT SIZE IS            %lld\n", btree->size);

    fprintf(log, "</font><tt>\n");

    char picture_name[MAX_PICTURE_NAME] = "";

    sprintf(picture_name, "%s%ld.%s", PICTURE_NAME, doing, PICTURE_CODE_EXPANSION);

    tree_print_picture(btree, picture_name);

    strncat(picture_name, ".", MAX_PICTURE_NAME);
    strncat(picture_name, PICTURE_EXPANSION, MAX_PICTURE_NAME);

    fprintf(log, "<img src=%s>", picture_name);

    fprintf(log, "\n");

    fclose(log);

    doing++;
}

tree_code tree_verifier(Binary_tree *btree, const tree_functions requester)
{
    tree_code error = tree_check_pointer(btree);
    if (error != TREE_OK)
    {
        ASSERTION(error);
        tree_dump(btree, error, requester);
        return error;
    }

    long long  connections_number = btree->capacity - btree->size;
    long long *free_indexes       = (long long*)calloc(btree->capacity, sizeof(long long));

    long long index = btree->first_free;
    for (int i = 0; i < connections_number; i++)
    {
        if (free_indexes[index])
        {
            ASSERTION(TREE_CONNECT_ERROR);
            tree_dump(btree, TREE_CONNECT_ERROR, requester);
            return TREE_CONNECT_ERROR;
        }
        free_indexes[index]++;

        index = btree->tree[index].next;
    }
    free(free_indexes);

    if (btree->tree[btree->root_index].parent != ABSENT)
    {
        ASSERTION(TREE_CONNECT_ERROR);
        tree_dump(btree, TREE_CONNECT_ERROR, requester);
        return TREE_CONNECT_ERROR;
    }

    tree_check_connections(btree, btree->root_index, &error);
    if (error != TREE_OK)
    {
        ASSERTION(TREE_CONNECT_ERROR);
        tree_dump(btree, TREE_CONNECT_ERROR, requester);
        return TREE_CONNECT_ERROR;
    }

    return TREE_OK;
}

