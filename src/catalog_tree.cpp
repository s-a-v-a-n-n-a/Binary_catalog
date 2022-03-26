#pragma once

#include "catalog_tree.h"

const char* tree_state_text[]
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

const char* TREE_FUNCTION_IDENTIFIERS[] =
{
    "CONSTRUCTION",
    "DESTRUCTION",
    "INSERTION",
    "RESIZING",
    "REMOVING",
};

#define ASSERTION(code)                                                  \
    fprintf(stderr, "-----------------!WARNING!----------------\n");     \
    fprintf(stderr, "IN FILE %s\nIN LINE %d\n", __FILE__, __LINE__);     \
    tree_print_errors(code);

//#define DEBUG


Catalog_tree *tree_init(FILE *input, const long long amount)
{
    Catalog_tree *ctree = (Catalog_tree*)calloc(1, sizeof(Catalog_tree));
    if (!ctree)
    {
        ASSERTION(TREE_NO_MEMORY);
        tree_dump(ctree, TREE_NO_MEMORY, CONSTRUCTION);
        return NULL;
    }

    if (tree_construct(ctree, input, amount) != TREE_OK)
    {
        tree_destruct(ctree);
        return NULL;
    }

	return ctree;
}

void tree_delete(Catalog_tree *ctree)
{
    tree_destruct(ctree);

    if (ctree)
        free(ctree);
}

tree_code tree_construct(Catalog_tree *ctree, FILE *input, const long long amount)
{
    ctree->tree = (Node*)calloc(amount, sizeof(Node));
    if (!ctree->tree)
    {
        ASSERTION(TREE_NO_MEMORY);
        tree_dump(ctree, TREE_NO_MEMORY, CONSTRUCTION);
        return TREE_NO_MEMORY;
    }
    ctree->capacity = amount;
    ctree->size     = 0;

	ctree->tree[0].parent    = ABSENT;
	ctree->tree[0].right_son = ABSENT;
	ctree->tree[0].left_son  = ABSENT;
    ctree->tree[0].way       = YES;

    ctree->root_index = 0;
	ctree->first_free = 0;

	for (long long i = 0; i < amount; i++)
	{
        ctree->tree[i].next = i + 1;
	}

	ctree->catalog = text_init(input);

	return TREE_OK;
}

void tree_destruct(Catalog_tree *ctree)
{
    if (ctree->catalog)
        text_delete(ctree->catalog);
    if (ctree->tree)
        free(ctree->tree);
}

long long tree_parent(Catalog_tree* ctree, long long index)
{
    return ctree->tree[index].parent;
}

long long tree_right_son(Catalog_tree *ctree, long long index)
{
    return ctree->tree[index].right_son;
}

long long tree_left_son(Catalog_tree *ctree, long long index)
{
    return ctree->tree[index].left_son;
}

size_t tree_message_length(Catalog_tree* ctree, long long index)
{
    return ctree->tree[index].message_length;
}

char *tree_message_beginnig(Catalog_tree *ctree, long long index)
{
    return ctree->catalog->buffer + ctree->tree[index].length_from_beginning;
}

int tree_if_lief(Catalog_tree *ctree, long long index)
{
    if (index > ctree->size || index < ABSENT)
        return 0;

    if (tree_right_son(ctree, index) == ABSENT && tree_left_son(ctree, index) == ABSENT)
        return 1;
    else
        return 0;
}

tree_code tree_resize(Catalog_tree *ctree, const double coefficient)
{
    size_t new_capacity = ctree->capacity * coefficient;

    Node *new_tree = (Node*)realloc(ctree->tree, sizeof(Node) * new_capacity);
    if (!new_tree)
    {
        ASSERTION(TREE_NO_MEMORY);
        tree_dump(ctree, TREE_NO_MEMORY, RESIZING);
        return TREE_NO_MEMORY;
    }

    ctree->tree = new_tree;

    if (coefficient > 1)
    {
        for (long long i = ctree->capacity; i < new_capacity; i++)
        {
            ctree->tree[i].next = i + 1;
        }
    }

    ctree->capacity = new_capacity;

    return TREE_OK;
}

long long tree_search(Catalog_tree *ctree, const char *message)
{   
    size_t mes_len = strlen(message);

    for (long long i = 0; i < ctree->size; i++)
    {
        long long length = tree_message_length(ctree, i);
        if (mes_len < length)
            continue;

        char *tree_message = tree_message_beginnig(ctree, i);
        for (size_t j = 0; j <= length; j++)
        {
            if (*(tree_message + j) != message[j])
                break;

            if (j == length)
                return i;
        }
    }

    return ABSENT;
}

tree_code tree_insert(Catalog_tree *ctree, long long *index_after, char answer, size_t length_from_begin, size_t message_length)
{
	if (*index_after > ctree->size + 1)
    {
        ASSERTION(TREE_OVERFLOW);
        tree_dump(ctree, TREE_OVERFLOW, INSERTION);
        return TREE_OVERFLOW;
    }

    if (ctree->size + 1 >= ctree->capacity)
    {
        tree_code error = tree_resize(ctree, RESIZE_UP);
        if (error != TREE_OK)
            return error;
    }

	ctree->tree[ctree->first_free].length_from_beginning = length_from_begin;
    ctree->tree[ctree->first_free].message_length        = message_length;
	if (*index_after == ABSENT)
	{
		ctree->tree[ctree->root_index].parent    = ABSENT;
	}
	else
	{
        ctree->tree[ctree->first_free].parent    = *index_after;

        if (answer == YES)
        {
            ctree->tree[*index_after].right_son = ctree->first_free;
            ctree->tree[ctree->first_free].way  = YES;
        }
        else
        {
            ctree->tree[*index_after].left_son = ctree->first_free;
            ctree->tree[ctree->first_free].way = NO;
        }
    }
    ctree->tree[ctree->first_free].right_son = ABSENT;
    ctree->tree[ctree->first_free].left_son  = ABSENT;

    *index_after      = ctree->first_free;
    ctree->first_free = ctree->tree[ctree->first_free].next;

    ctree->size++;

    return TREE_OK;
}

static void tree_remove_lief(Catalog_tree *ctree, long long index_after, char answer)
{
    if (index_after == ABSENT)
	{
        ctree->tree[ctree->root_index].next = ctree->first_free;
        ctree->first_free = ctree->root_index;
	}
	else
	{
		ctree->tree[index_after].next = ctree->first_free;
        ctree->first_free = index_after;

        if (answer == YES)
            ctree->tree[index_after].right_son = ABSENT;
        else
            ctree->tree[index_after].left_son = ABSENT;
	}

	ctree->size--;
}

static void tree_remove_branch(Catalog_tree *ctree, long long branch_base)
{
    long long parent = tree_parent(ctree, branch_base);
    long long right  = tree_right_son(ctree, branch_base);
    long long left   = tree_left_son(ctree, branch_base);
    if (tree_if_lief(ctree, branch_base) && ctree->tree[branch_base].parent >= 0)
    {
        if (tree_right_son(ctree, parent) == branch_base)
        {
            tree_remove_lief(ctree, parent, YES);
            return;
        }
        else
        {
            tree_remove_lief(ctree, parent, NO);
            return;
        }
    }

    if (right != ABSENT)
        tree_remove_branch(ctree, right);
    if (left != ABSENT)
        tree_remove_branch(ctree, left);

    if (parent == ABSENT)
    {
        tree_remove_lief(ctree, ABSENT, YES);
        return;
    }
    else
    {
        if (ctree->tree[branch_base].way == YES)
        {
            tree_remove_lief(ctree, parent, YES);
            return;
        }
        else
        {
            tree_remove_lief(ctree, parent, NO);
            return;
        }
    }
}

tree_code tree_clean_branch(Catalog_tree *ctree, long long branch_base)
{
    if (branch_base > ctree->size || branch_base < ABSENT)
    {
        ASSERTION(TREE_UNDERFLOW);
        tree_dump(ctree, TREE_UNDERFLOW, REMOVING);
        return TREE_UNDERFLOW;
    }
    tree_code error = TREE_OK;

    tree_remove_branch(ctree, branch_base);

    if (ctree->size < ctree->capacity / 4)
    {
        tree_code resize_error = tree_resize(ctree, RESIZE_DOWN);
        if (resize_error != TREE_OK)
            return resize_error;
    }

    return error;
}

tree_code tree_check_pointer(Catalog_tree *ctree)
{
    if (!ctree || !ctree->tree)
        return TREE_NULL;

    if ((size_t)(ctree) <= 4096 || (size_t)(ctree->tree) <= 4096)
        return TREE_SEG_FAULT;

    return TREE_OK;
}

void tree_check_connections(Catalog_tree *ctree, long long index, tree_code *error)
{
    long long parent = tree_parent(ctree, index);
    long long right  = tree_right_son(ctree, index);
    long long left   = tree_left_son(ctree, index);
    if (right == ABSENT && left == ABSENT)
        return;
    else if (right < ABSENT || left < ABSENT)
    {
        *error = TREE_CONNECT_ERROR;
        return;
    }

    if (right != ABSENT)
    {
        if (index != tree_parent(ctree, right))
        {
            *error = TREE_CONNECT_ERROR;
            return;
        }

        tree_check_connections(ctree, right, error);
    }
    if (left != ABSENT)
    {
        if (index != tree_parent(ctree, left))
        {
            *error = TREE_CONNECT_ERROR;
            return;
        }

        tree_check_connections(ctree, left, error);
    }
}

void tree_print_errors(tree_code code)
{
    printf("Error: %s\n", tree_state_text[code]);
}

void tree_dump(Catalog_tree *ctree, tree_code code, const tree_functions function)
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

    fprintf(log, "CURRENT CAPACITY IS %lld\n", ctree->capacity);
    fprintf(log, "CURRENT SIZE IS            %lld\n", ctree->size);

    fprintf(log, "</font><tt>\n");

    char picture_name[MAX_PICTURE_NAME] = " ";

    sprintf(picture_name, "%s%ld.%s", PICTURE_NAME, doing, PICTURE_CODE_EXPANSION);

    tree_print_picture(ctree, picture_name);

    strncat(picture_name, ".", MAX_PICTURE_NAME);
    strncat(picture_name, PICTURE_EXPANSION, MAX_PICTURE_NAME);

    fprintf(log, "<img src=%s>", picture_name);

    fprintf(log, "\n");

    fclose(log);

    doing++;
}

tree_code tree_verifier(Catalog_tree *ctree, const tree_functions requester)
{
    tree_code error = tree_check_pointer(ctree);
    if (error != TREE_OK)
    {
        ASSERTION(error);
        tree_dump(ctree, error, requester);
        return error;
    }

    long long  connections_number = ctree->capacity - ctree->size;
    long long *free_indexes       = (long long*)calloc(ctree->capacity, sizeof(long long));

    long long index = ctree->first_free;
    for (int i = 0; i < connections_number; i++)
    {
        if (free_indexes[index])
        {
            ASSERTION(TREE_CONNECT_ERROR);
            tree_dump(ctree, TREE_CONNECT_ERROR, requester);
            return TREE_CONNECT_ERROR;
        }
        free_indexes[index]++;

        index = ctree->tree[index].next;
    }
    free(free_indexes);

    if (ctree->tree[ctree->root_index].parent != ABSENT)
    {
        ASSERTION(TREE_CONNECT_ERROR);
        tree_dump(ctree, TREE_CONNECT_ERROR, requester);
        return TREE_CONNECT_ERROR;
    }

    tree_check_connections(ctree, ctree->root_index, &error);
    if (error != TREE_OK)
    {
        ASSERTION(TREE_CONNECT_ERROR);
        tree_dump(ctree, TREE_CONNECT_ERROR, requester);
        return TREE_CONNECT_ERROR;
    }

    return TREE_OK;
}

void tree_dot_call(const char *name_file, const char *expansion)
{
    char temp[512];
    sprintf(temp, "dot %s -T%s -O", name_file, expansion);
    system((char*)temp);
}


void tree_print_message_in_file(Catalog_tree *ctree, long long index, FILE *picture)
{
    long long length  = tree_message_length(ctree, index);
    char*     message = tree_message_beginnig(ctree, index);
    for (size_t i = 0; i <= length; i++)
        fprintf(picture, "%c", message[i]);
}

void tree_print_picture_nodes(Catalog_tree *ctree, long long index, FILE *picture)
{
    #ifdef DEBUG
    fprintf(picture, "  nod%lld[shape=\"none\", ", index);
    fprintf(picture, "label = <<table border = \"0\" cellborder = \"1\" cellspacing = \"0\">\n");
    fprintf(picture, "      <tr>\n      <td colspan = \"2\" bgcolor = \"HotPink\">parent %lld</td>\n    </tr>\n", ctree->tree[index].parent);
    fprintf(picture, "      <tr>\n      <td colspan = \"2\" bgcolor = \"%s\">index %lld</td>\n     </tr>\n", COLOR_FILL_DIFFERENCE, index);
    fprintf(picture, "      <tr>\n      <td colspan = \"2\" bgcolor = \"%s\">", COLOR_FILL_DIFFERENCE);
    tree_print_message_in_file(ctree, index, picture);
    fprintf(picture, "</td>\n     </tr>\n");
    fprintf(picture, "      <tr>\n");
    fprintf(picture, "          <td bgcolor = \"%s\">NO %lld</td>\n", COLOR_FILL_SONS, ctree->tree[index].left_son);
    fprintf(picture, "          <td bgcolor = \"%s\">YES %lld</td>\n", COLOR_FILL_SONS, ctree->tree[index].right_son);
    fprintf(picture, "      </tr>\n");
    fprintf(picture, "      </table>>];\n");
    #else
    fprintf(picture, "  nod%lld[shape=\"none\", ", index);
    if (tree_if_lief(ctree, index))
        fprintf(picture, "fillcolor =  \"%s\", style=filled", COLOR_FILL_CHARACTER);
    else
        fprintf(picture, "fillcolor =  \"%s\", style=filled", COLOR_FILL_DIFFERENCE);
    fprintf(picture, "  label = \"");
    tree_print_message_in_file(ctree, index, picture);
    fprintf(picture, "\"];\n");
    #endif
}

void tree_generate_picture_nodes(Catalog_tree *ctree, long long index, FILE *picture)
{
    long long right = tree_right_son(ctree, index);
    long long left  = tree_left_son(ctree, index);
    if (left < 0 && right < 0)
    {
        tree_print_picture_nodes(ctree, index, picture);

        return;
    }

    tree_print_picture_nodes(ctree, index, picture);

    if (left >= 0)
    {
        tree_generate_picture_nodes(ctree, left, picture);
    }
    if (right >= 0)
    {
        tree_generate_picture_nodes(ctree, right, picture);
    }

    return;
}

void tree_print_picture_sequence(Catalog_tree *ctree, long long index, FILE *picture)
{
    long long right = tree_right_son(ctree, index);
    long long left  = tree_left_son(ctree, index);
    if (left < 0 && right < 0)
    {
        return;
    }

    if (left != ABSENT)
    {
        tree_print_picture_sequence(ctree, left, picture);
        fprintf(picture, "      nod%lld->nod%lld[label=\"NO\"];\n", index, left);
    }
    if (right != ABSENT)
    {
        tree_print_picture_sequence(ctree, right, picture);
        fprintf(picture, "      nod%lld->nod%lld[label=\"YES\"];\n", index, right);
    }

    return;
}

void tree_print_picture(Catalog_tree *ctree, const char *picture_name)
{
    FILE* picture = fopen(picture_name, "wb");

    fprintf(picture, "digraph\n{\n  rankdir = TB;\n");
    tree_generate_picture_nodes(ctree, ctree->root_index, picture);
    fprintf(picture, "   ");
    tree_print_picture_sequence(ctree, ctree->root_index, picture);
    fprintf(picture, "}");

    fclose(picture);

    tree_dot_call(picture_name, PICTURE_EXPANSION);
}


