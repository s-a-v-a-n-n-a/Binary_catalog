#include <stdio.h>
#include "TXLib.h"

#include "binary_tree_consts.h"
#include "binary_tree_work_with_text.c"
#include "binary_tree.c"

void catalog_print_greeting();
void catalog_print_help();

void catalog_call_picture(Binary_tree *btree);

void start_game(Binary_tree *btree, long long index);

int main(const int argc, const char* argv[])
{
    char command[255] = "";
    char input[MAX_MESSAGE_LEN] = "";

    Binary_tree *not_fake_akinator = NULL;
    if (argc > 1)
        not_fake_akinator = tree_read_from_file(argv[1]);
    else
        not_fake_akinator = tree_read_from_file(DEFAULT_FILE_NAME);

    catalog_print_greeting();
    catalog_print_help();

    //Кажется, есть смысл сравнивать строчки, потому что если пользователь введёт бяку, а я среагирую, будет больно
    while(scanf("%8s", command) == 1)
    {
        if (command[0] == 'h')
        {
            catalog_print_help();
        }
        else if (command[0] == 'g')
        {
            start_game(not_fake_akinator, not_fake_akinator->root_index);
        }
        else if (command[0] == 'd')
        {
            char symbol = 0;

            scanf("%c", &symbol);
            scanf("%[^\n]%*c", input);

            tree_make_definition(not_fake_akinator, input);
            printf("Re>");
        }
        else if (command[0] == 'c')
        {
            printf("Re>I can not do it yet..... :(\n");
        }
        else if (command[0] == 'p')
        {
            catalog_call_picture(not_fake_akinator);

            printf("Re>");
        }
        else if (command[0] == 'e')
        {
            printf("\nRe>Goodbye...I will miss you...\n");
            break;
        }
        else
        {
            printf("\nRe>I do not know what you want from me :(\n");
            printf("\nRe>.....maybe you wand to play with me :D (input \"game\")\n");
        }
    }
    FILE *catalog = NULL;
    if (argc > 1)
        catalog = fopen(argv[1], "wb");
    else
        catalog = fopen(DEFAULT_FILE_NAME, "wb");

    tree_write_in_file(not_fake_akinator, not_fake_akinator->root_index, catalog, 0);
    fclose(catalog);

    tree_delete(not_fake_akinator);
    return 0;
}

void catalog_print_greeting()
{
    printf("Re>Hello! I am not-fake-akinator!\nRe>There are som commands to interact with me:\n");
}

void catalog_print_help()
{
    printf("Re>help                                  -if you have forgotten how to interact with me\n");
    printf("Re>game                                  -if you want to play with me\n");
    printf("Re>define <your input>                   -if you want me to define your character\n");
    printf("Re>compare <first input> <second input>  -if you want me to compare two characters\n");
    printf("Re>picture                               -if you want to see the appearance of the tree\n");
    printf("Re>exit                                  -if you want to say goodbye to me :(\nRe>");
}

void catalog_call_picture(Binary_tree *btree)
{
    tree_print_picture(btree, "tree_picture.txt");

    char temp[512];
    sprintf(temp, "%s.%s.%s", PICTURE_NAME, PICTURE_CODE_EXPANSION, PICTURE_EXPANSION);
    system((char*)temp);
}

void start_game(Binary_tree *btree, long long index)
{
    char answer[MAX_ANSWER_LEN] = "";

    if (btree->tree[index].right_son == ABSENT && btree->tree[index].right_son == ABSENT)
    {
        printf("Re>Is it...%s?\nRe>", btree->tree[index].message);
        scanf("%"MA"s", answer);

        do
        {
            if (answer[0] == 'y' || answer[0] == 'Y')
            {
                printf("\nRe>I knew it! I know everything!\nRe>");
                return;
            }
            else if (answer[0] == 'n' || answer[0] == 'N')
            {
                char new_character[MAX_MESSAGE_LEN] = "";
                char difference   [MAX_MESSAGE_LEN] = "";

                char symbol = 0;

                printf("\nRe>Oh... So what is it?\nRe>");
                scanf("%c", &symbol);
                scanf("%[^\n]%*c", new_character);     //Что это??????

                if (strcmp(btree->tree[index].message, new_character))
                {
                    printf("\nRe>And what is the difference between %s and %s?\nRe>", new_character, btree->tree[index].message);

                    int symbol_num = 0;

                    scanf("%[^\n]%*c", difference);

                    tree_add_feature(btree, index, new_character, difference);
                    printf("Re>");
                }
                else
                    printf("\nRe>But I said exactly this! You can not deceive me! I know everything!!!\nRe>");
                return;
            }
            else
                printf("\nRe>I do not know what does it mean :(\nRe>");
        }
        while(answer[0] != 'y' || answer[0] != 'Y' || answer[0] == 'n' || answer[0] == 'N');
    }

    printf("Re>It %s?\nRe>", btree->tree[index].message);
    scanf("%"MA"s", answer);

    do
    {
        if (answer[0] == 'y' || answer[0] == 'Y')
        {
            start_game(btree, btree->tree[index].right_son);
            return;
        }
        else if (answer[0] == 'n' || answer[0] == 'N')
        {
            start_game(btree, btree->tree[index].left_son);
            return;
        }
        else
            printf("\nRe>I do not know what does it mean :(\nRe>");
    }
    while(answer[0] != 'y' || answer[0] != 'Y' || answer[0] == 'n' || answer[0] == 'N');
}
