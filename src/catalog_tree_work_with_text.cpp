#pragma once

#include "catalog_tree_work_with_text.h"
#include "catalog_tree.h"


size_t tree_count_nodes(Catalog *base)
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

static char *tree_skip_space(char *pointer, size_t *length)
{
     if (!pointer)
        return NULL;

     while(*pointer != '"' && *pointer != '[')
     {
        pointer++;
        (*length)++;

        if (*pointer == '\0')
            break;
     }
     return pointer;
}

static char tree_look_for_bracket(char **pointer, size_t *length)
{
    if (!*pointer)
        return 0;

    while(**pointer != '"' && **pointer != '[')
    {
        (*pointer)++;
        (*length)++;

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

Catalog_tree *tree_read_from_base(FILE *input)
{
     Catalog_tree *new_tree = tree_init(input, DEF_SIZE);

     size_t nodes_amount = tree_count_nodes(new_tree->catalog);

     size_t message_length = 0;

     char  *current_stop = new_tree->catalog->buffer;
     size_t length_went  = 0;

     long long current_index  = -1;

     char went_global_right = 0;
     char went_local_right  = 0;

     for (int i = 0; i < nodes_amount; i++)
     {
         message_length = 0;

         current_stop = tree_skip_space(current_stop, &length_went);

         if (current_stop)
         {
            if (*current_stop == '"')
            {
                current_stop++;
                length_went++;
                while (*current_stop != '"' && *current_stop != '\0')
                {
                    current_stop++;
                    length_went++;
                    message_length++;
                }

                if (current_index == ABSENT && !went_global_right)
                {
                    tree_insert(new_tree, &current_index, YES, length_went - message_length, message_length - 1);
                    went_global_right++;
                }
                else if (new_tree->tree[current_index].right_son == ABSENT)
                {
                    tree_insert(new_tree, &current_index, YES, length_went - message_length, message_length - 1);
                    went_local_right++;
                }
                else if (new_tree->tree[current_index].left_son == ABSENT)
                {
                    tree_insert(new_tree, &current_index, NO, length_went - message_length, message_length - 1);
                    went_local_right = 0;
                }
                else
                {
                    while (new_tree->tree[current_index].right_son != ABSENT &&
                           new_tree->tree[current_index].left_son != ABSENT)
                        current_index = new_tree->tree[current_index].parent;

                    if (new_tree->tree[current_index].right_son == ABSENT)
                    {
                        tree_insert(new_tree, &current_index, YES, length_went - message_length, message_length - 1);
                        went_local_right++;
                    }
                    else if (new_tree->tree[current_index].left_son == ABSENT)
                    {
                        tree_insert(new_tree, &current_index, NO, length_went - message_length, message_length - 1);
                        went_local_right = 0;
                    }
                }
            }
            current_stop++;
            length_went++;
            int result = tree_look_for_bracket(&current_stop, &length_went);
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
                length_went++;
            }
         }
         else
            break;
     }

     return new_tree;
}

Catalog_tree *tree_read_from_file(const char *file_name)
{
    FILE *input = text_read(file_name);

    Catalog_tree *new_tree = tree_read_from_base(input);

    fclose(input);

    return new_tree;
}

void tree_print_padding(FILE *file_name, int padding)
{
    for (int i = 0; i < padding; i++)
    {
        fprintf(file_name, "    ");
    }
}

void tree_write_in_file(Catalog_tree *ctree, long long index, FILE *file_name, int padding)
{
    if (tree_if_lief(ctree, index))
    {
        tree_print_padding(file_name, padding);
        fprintf(file_name, "\"");
        tree_print_message_in_file(ctree, index, file_name);
        fprintf(file_name, "\"\n");

        return;
    }

    tree_print_padding(file_name, padding);

    fprintf(file_name, "\"");
    tree_print_message_in_file(ctree, index, file_name);
    fprintf(file_name, "\"\n");

    tree_print_padding(file_name, padding);
    fprintf(file_name, "[\n");

    if (ctree->tree[index].right_son != ABSENT)
    {
        tree_write_in_file(ctree, ctree->tree[index].right_son, file_name, padding + 1);
    }
    if (ctree->tree[index].left_son != ABSENT)
    {
        tree_write_in_file(ctree, ctree->tree[index].left_son, file_name, padding + 1);
    }

    tree_print_padding(file_name, padding);
    fprintf(file_name, "]\n");
}
