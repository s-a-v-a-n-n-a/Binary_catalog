#include "binary_tree_picture.h"

//#define DEBUG


void tree_dot_call(const char *name_file, const char *expansion)
{
    char temp[512];
    sprintf(temp, "dot %s -T%s -O", name_file, expansion);
    system((char*)temp);
}

void tree_print_picture_nodes(Binary_tree *btree, long long index, FILE *picture)
{
    #ifdef DEBUG
    fprintf(picture, "  nod%lld[shape=\"none\", ", index);
    fprintf(picture, "label = <<table border = \"0\" cellborder = \"1\" cellspacing = \"0\">\n");
    fprintf(picture, "      <tr>\n      <td colspan = \"2\" bgcolor = \"HotPink\">parent %lld</td>\n    </tr>\n", btree->tree[index].parent);
    fprintf(picture, "      <tr>\n      <td colspan = \"2\" bgcolor = \"#FEC9E3\">index %lld</td>\n     </tr>\n", index);
    fprintf(picture, "      <tr>\n      <td colspan = \"2\" bgcolor = \"#FEC9E3\">%s</td>\n     </tr>\n", btree->tree[index].message);
    fprintf(picture, "      <tr>\n");
    fprintf(picture, "          <td bgcolor = \"HotPink\">NO %lld</td>\n", btree->tree[index].left_son);
    fprintf(picture, "          <td bgcolor = \"HotPink\">YES %lld</td>\n", btree->tree[index].right_son);
    fprintf(picture, "      </tr>\n");
    fprintf(picture, "      </table>>];\n");
    #else
    fprintf(picture, "  nod%lld[shape=\"none\", fillcolor =  \"#FEC9E3\", style=filled", index);
    fprintf(picture, "  label = \"%s\"];\n", btree->tree[index].message);
    #endif
}

void tree_generate_picture_nodes(Binary_tree *btree, long long index, FILE *picture)
{
    if (btree->tree[index].left_son < 0 && btree->tree[index].right_son < 0)
    {
        tree_print_picture_nodes(btree, index, picture);

        return;
    }

    tree_print_picture_nodes(btree, index, picture);

    if (btree->tree[index].left_son >= 0)
    {
        tree_generate_picture_nodes(btree, btree->tree[index].left_son, picture);
    }
    if (btree->tree[index].right_son >= 0)
    {
        tree_generate_picture_nodes(btree, btree->tree[index].right_son, picture);
    }

    return;
}

void tree_print_picture_sequence(Binary_tree *btree, long long index, FILE *picture)
{
    if (btree->tree[index].left_son < 0 && btree->tree[index].right_son < 0)
    {
        return;
    }

    if (btree->tree[index].left_son != ABSENT)
    {
        tree_print_picture_sequence(btree, btree->tree[index].left_son, picture);
        fprintf(picture, "      nod%lld->nod%lld[label=\"NO\"];\n", index, btree->tree[index].left_son);
    }
    if (btree->tree[index].right_son != ABSENT)
    {
        tree_print_picture_sequence(btree, btree->tree[index].right_son, picture);
        fprintf(picture, "      nod%lld->nod%lld[label=\"YES\"];\n", index, btree->tree[index].right_son);
    }

    return;
}

void tree_print_picture(Binary_tree *btree, const char *picture_name)
{
    FILE* picture = fopen(picture_name, "wb");

    fprintf(picture, "digraph\n{\n  rankdir = TB;\n");
    tree_generate_picture_nodes(btree, btree->root_index, picture);
    fprintf(picture, "   ");
    tree_print_picture_sequence(btree, btree->root_index, picture);
    fprintf(picture, "}");

    fclose(picture);

    tree_dot_call(picture_name, PICTURE_EXPANSION);
}
