#pragma once

#include "binary_tree_work_with_text.h"
#include "binary_tree.h"
#include "binary_tree.c"


void tree_add_feature(Binary_tree *btree, long long index, const char *new_node, const char *feature)
{
    char last_node[MAX_MESSAGE_LEN] = "";
    memcpy(last_node, btree->tree[index].message, MAX_MESSAGE_LEN);

    long long index_after = btree->tree[index].parent;

    if (index_after == ABSENT)
    {
        tree_insert(btree, &index_after, YES, feature);

        tree_insert(btree, &index_after, YES, new_node);
        index_after = btree->tree[index_after].parent;

        tree_insert(btree, &index_after, NO, last_node);

        return;
    }

    int answer = 0;
    if (index == btree->tree[btree->tree[index].parent].right_son)
        answer = YES;
    else
        answer = NO;

    //tree_clean_branch(btree, index);

    tree_insert(btree, &index_after, answer, feature);

    tree_insert(btree, &index_after, YES, new_node);
    index_after = btree->tree[index_after].parent;

    tree_insert(btree, &index_after, NO, last_node);
}

void tree_print_definition(Binary_tree *btree, long long place, long long from)
{
    if (place == btree->root_index)
    {
        if (from ==  btree->tree[btree->root_index].right_son)
            printf("%s", btree->tree[place].message);
        else
            printf("not %s", btree->tree[place].message);
        return;
    }

    tree_print_definition(btree, btree->tree[place].parent, place);

    if (from ==  btree->tree[place].right_son)
        printf(", %s", btree->tree[place].message);
    else
        printf(", not %s", btree->tree[place].message);
}

void tree_make_definition(Binary_tree *btree, const char *node)
{
    long long place = tree_search(btree, node);
    if (place == ABSENT)
    {
        printf("Re>No %s found:(\n", node);
        return;
    }
    printf("Re>%s ", node);
    tree_print_definition(btree, btree->tree[place].parent, place);
    printf("\n");
}

size_t tree_count_nodes(File_text *base)
{
     size_t nodes_counter        = 0;
     char   node_open_identifier = 0;
     for (size_t i = 0; i < base->length; i++)
     {
        if (base->buffer[i] == '"' && !node_open_identifier)
        {
            nodes_counter++;
            node_open_identifier = 1;
        }
        else if (base->buffer[i] == '"' && node_open_identifier)
            node_open_identifier = 0;
     }

     return nodes_counter;
}

static char *tree_skip_space(char *pointer)
{
     if (!pointer)
        return NULL;

     while(*pointer != '"' && *pointer != '[')
     {
        pointer++;

        if (*pointer == '\0')
            break;
     }
     return pointer;
}

static char tree_look_for_bracket(char **pointer)
{
    if (!*pointer)
        return 0;

    while(**pointer != '"' && **pointer != '[')
    {
        (*pointer)++;

        if (**pointer == '\0')
        {
            *pointer = NULL;
            return 0;
        }
    }
    if (**pointer == '[')
        return 1;
    else
        return 0;
}

Binary_tree *tree_read_from_base(File_text *base)
{
     size_t nodes_amount = tree_count_nodes(base);

     Binary_tree *new_tree = tree_init(nodes_amount);

     char     *message        = NULL;
     long long message_length = 0;

     char *current_stop = base->buffer;

     long long current_index  = -1;

     char went_global_right = 0;
     char went_local_right  = 0;

     for (int i = 0; i < nodes_amount; i++)
     {
         message_length = 0;

         current_stop = tree_skip_space(current_stop);
         //printf("current stop %c\n", current_stop);

         if (current_stop)
         {
            if (*current_stop == '"')
            {
                current_stop++;
                while (*current_stop != '"' && *current_stop != '\0')
                {
                    current_stop++;
                    message_length++;
                }
                message = (char*)calloc(message_length + 1, sizeof(char));
                memcpy(message, current_stop - message_length, message_length);

                //printf("%s\n", message);

                if (current_index == ABSENT && !went_global_right)
                {
                    tree_insert(new_tree, &current_index, YES, message);
                    //printf("begin %s\n", message);
                    went_global_right++;
                }
                else if (current_index == ABSENT && went_global_right == 1)
                {
                    tree_insert(new_tree, &current_index, NO, message);
                    went_global_right++;
                }
                else if (new_tree->tree[current_index].right_son == ABSENT)
                {
                    tree_insert(new_tree, &current_index, YES, message);
                    went_local_right++;
                    //printf("right %s, parent %d\n", message, new_tree->tree[current_index].parent);
                }
                else if (new_tree->tree[current_index].left_son == ABSENT)
                {
                    tree_insert(new_tree, &current_index, NO, message);
                    went_local_right = 0;
                    //printf("left %s, parent %d\n", message, new_tree->tree[current_index].parent);
                }
                else
                {
                    while (new_tree->tree[current_index].right_son != ABSENT &&
                           new_tree->tree[current_index].left_son != ABSENT)
                        current_index = new_tree->tree[current_index].parent;

                    if (new_tree->tree[current_index].right_son == ABSENT)
                    {
                        tree_insert(new_tree, &current_index, YES, message);
                        went_local_right++;
                        //printf("right %s, parent %d\n", message, new_tree->tree[current_index].parent);
                    }
                    else if (new_tree->tree[current_index].left_son == ABSENT)
                    {
                        tree_insert(new_tree, &current_index, NO, message);
                        went_local_right = 0;
                        //printf("left %s, parent %d\n", message, new_tree->tree[current_index].parent);
                    }
                }
                free(message);
            }
            current_stop++;
            int result = tree_look_for_bracket(&current_stop);
            //printf("result %d\n", result);
            if (!result)
            {
                if (current_index != ABSENT)
                    current_index = new_tree->tree[current_index].parent;
            }
            else
            {
                went_local_right = 0;
                if (!current_stop)
                    break;
                current_stop++;
            }
         }
         else
            break;

         //tree_dump(new_tree, TREE_OK, CONSTRUCTION);
     }

     return new_tree;
}

Binary_tree *tree_read_from_file(const char *file_name)
{
    File_text *base = text_read(file_name);

    return tree_read_from_base(base);
}

void tree_print_padding(FILE *file_name, int padding)
{
    for (int i = 0; i < padding; i++)
    {
        fprintf(file_name, "    ");
    }
}

void tree_write_in_file(Binary_tree *btree, long long index, FILE *file_name, int padding)
{
    if (btree->tree[index].right_son == ABSENT && btree->tree[index].right_son == ABSENT)
    {
        tree_print_padding(file_name, padding);
        fprintf(file_name, "\"%s\"\n", btree->tree[index].message);

        return;
    }

    tree_print_padding(file_name, padding);
    fprintf(file_name, "\"%s\"\n", btree->tree[index].message);

    tree_print_padding(file_name, padding);
    fprintf(file_name, "[\n");

    if (btree->tree[index].right_son != ABSENT)
    {
        tree_write_in_file(btree, btree->tree[index].right_son, file_name, padding + 1);
    }
    if (btree->tree[index].left_son != ABSENT)
    {
        tree_write_in_file(btree, btree->tree[index].left_son, file_name, padding + 1);
    }

    tree_print_padding(file_name, padding);
    fprintf(file_name, "]\n");
}
