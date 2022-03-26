#pragma once

#include "catalog_tree_user_work.h"

void catalog_inform_user(const char *information)
{
    catalog_print_phrase(information, FIRST_INPUT, END_OF_STRING);
    catalog_say_phrase  (information);
}

void catalog_ask_user(Catalog_tree *ctree, long long index, const char *question)
{
    catalog_print_message(ctree, index, question, QUESTION_MARK);
    catalog_say_message  (ctree, index, question, QUESTION_MARK);
}

void catalog_linking_words(const char *word, const char *mark)
{
    catalog_print_phrase(word, "", mark);
    catalog_say_phrase(word);
}

void catalog_main_words(Catalog_tree* ctree, long long index, char status_end)
{
    if (status_end == BEGIN_OF_SENTENCE)
    {
        catalog_print_message(ctree, index, FIRST_INPUT, END_OF_WORD);
        catalog_say_message(ctree, index, "", END_OF_WORD);
    }
    else if (status_end == END_OF_SENTENCE)
    {
        catalog_print_message(ctree, index, "", END_OF_STRING);
        catalog_say_message(ctree, index, "", END_OF_WORD);
    }
    else if (status_end == LINKING_SENTENCE)
    {
        catalog_print_message(ctree, index, "", COMMA);
        catalog_say_message(ctree, index, "", COMMA);
    }
    else
    {
        catalog_print_message(ctree, index, "", END_OF_WORD);
        catalog_say_message(ctree, index, "", END_OF_WORD);
    }
}

static void catalog_print_message(Catalog_tree* ctree, long long index)
{
    long long mes_len = tree_message_length(ctree, index);
    char* message = tree_message_beginnig(ctree, index);
    for (size_t i = 0; i <= mes_len; i++)
        printf("%c", (message)[i]);
}

static void catalog_print_phrase(const char *phrase, const char *beginning, const char *ending)
{
    printf("%s%s%s", beginning, phrase, ending);
}

static void catalog_print_message(Catalog_tree *ctree, long long index, const char* before, const char* after)
{
    printf("%s", before);
    catalog_print_message(ctree, index);
    printf("%s", after);
}

static void catalog_say_phrase(const char *phrase)
{
    txSpeak("<speak version='1.0' xmlns='http://www.w3.org/2001/10/synthesis' xml:lang='EN'>"
        "<prosody rate=\"1.75\">"
            "%s"
        "</prosody>"
        "</speak>", phrase);
}

static void catalog_say_message(Catalog_tree *ctree, long long index, const char* before, const char* after)
{
    char message[MAX_MESSAGE_LEN] = "";
    memcpy(message, tree_message_beginnig(ctree, index), tree_message_length(ctree, index) + 1);

    char phrase[MAX_MESSAGE_LEN] = "";
    strcpy(phrase, "<speak version='1.0' xmlns='http://www.w3.org/2001/10/synthesis' xml:lang='EN'> <prosody rate = \"1.75\">");
    strcat(phrase, before);
    strcat(phrase, message);
    strcat(phrase, after);
    strcat(phrase, " </prosody> </speak>");

    txSpeak(phrase);
}

void catalog_print_help()
{
    printf("There are some commands to interact with me:\n");
    printf("[h]                                  -if you have forgotten how to interact with me\n");
    printf("[g]                                  -if you want to play with me\n");
    printf("[d] <your input>                     -if you want me to define your character\n");
    printf("[c] <first input> <second input>     -if you want me to compare two characters\n");
    printf("[p]                                  -if you want to see the appearance of the tree\n");
    printf("[e]                                  -if you want to say goodbye to me :(\n");

    //catalog_say_phrase("There are some commands to interact with me: Print h to see help, print g to play with me, print d and the character in triangular brackets to see its definition, print c and two characters in triangular brackets to compare them, print p to see the tree and print e to exit the program");
    catalog_say_phrase("I do not want to read it because it takes too much time");
}

void catalog_read_requests(Catalog_tree *ctree)
{
    char command = 0;

    char input[MAX_MESSAGE_LEN]         = "";
    char input_compare[MAX_MESSAGE_LEN] = "";

    char symbol      = 0;
    char first_brake = 0;
    
    printf(FIRST_INPUT);

    while (scanf("%c", &command) == 1)
    {
        scanf("%c", &symbol);
        switch (command)
        {
        case 'h':
            catalog_print_help();
            break;
        case 'g':
            catalog_game(ctree, ctree->root_index);
            break;
        case 'd':
            scanf("%c%[^>]%*c", &first_brake, input);
            scanf("%c", &symbol);

            catalog_make_definition(ctree, input);
            break;
        case 'c':
            scanf("%c%[^>]%*c", &first_brake, input);
            scanf("%c%c%[^>]%*c", &symbol, &first_brake, input_compare);
            scanf("%c", &symbol);

            catalog_make_comparison(ctree, input, input_compare);
            break;
        case 'p':
            catalog_call_picture(ctree);
            break;
        case 'e':
            catalog_inform_user(GOODBYE);
            return;
            break;
        default:
            catalog_inform_user(WRONG_INPUT);
            catalog_inform_user(HYPOTESIS);
            break;
        }

        printf(FIRST_INPUT);
    }
}

void catalog_call_picture(Catalog_tree *ctree)
{
    tree_print_picture(ctree, "tree_picture.txt");

    char temp[512];
    sprintf(temp, "%s.%s.%s", PICTURE_NAME, PICTURE_CODE_EXPANSION, PICTURE_EXPANSION);
    system((char*)temp);
}

void catalog_add_new_character(Catalog_tree *ctree, long long index, const char *character)
{
    char difference   [MAX_MESSAGE_LEN] = "";

    printf(">");
    scanf("%[^\n]%*c", difference);

    catalog_add_feature(ctree, index, character, difference);
}

int catalog_check_character_identity(Catalog_tree *ctree, long long index, const char *character)
{
    int    identical = 0;
    size_t mes_len   = strlen(character);
    size_t go_length = (mes_len < ctree->tree[index].message_length) ? mes_len : ctree->tree[index].message_length;

    for (size_t j = 0; j < go_length; j++)
    {
        if (*(ctree->catalog->buffer + ctree->tree[index].length_from_beginning + j) != character[j])
            break;
        identical = 1;
    }

    return identical;
}

char catalog_answer()
{
    char answer = 0;
    char symbol = 0;
    do
    {
        answer = toupper(getchar());
        symbol = getchar();
        if (answer != 'Y' && answer != 'N')
            catalog_inform_user(WRONG_INPUT);
    } while (answer != 'Y' && answer != 'N');

    return answer;
}

void catalog_game(Catalog_tree *ctree, long long index)
{
    char answer = 0;
    catalog_inform_user(DESCRIBE_EXPECTED_ANSWER);

    while (!tree_if_lief(ctree, index))
    {
        catalog_ask_user(ctree, index, FIND_IF_FEATURE_IS_TRUTH);

        printf(FIRST_INPUT);
        answer = catalog_answer();
        if (answer == 'Y')
            index = ctree->tree[index].right_son;
        else if (answer == 'N')
            index = ctree->tree[index].left_son;
    }

    catalog_ask_user(ctree, index, TRY_TO_GUESS);

    printf(FIRST_INPUT);
    answer = catalog_answer();
    if (answer == 'Y')
    {
        catalog_inform_user(KNOWING);
        return;
    }
    else if (answer == 'N')
    {
        catalog_inform_user(ASK_FOR_CHARACTER);

        char new_character[MAX_MESSAGE_LEN] = " ";

        printf(">");
        scanf("%[^\n]%*c", new_character);

        if (!catalog_check_character_identity(ctree, index, new_character))
        {
            catalog_inform_user(ASK_FOR_DESCRIPTION);
            catalog_add_new_character(ctree, index, new_character);
        }
        else
            catalog_inform_user(DECEIPT);
        return;
    }
}

void catalog_fprint_new(Catalog_tree *ctree, const char *file_name)
{
    FILE *catalog = NULL;

    catalog = fopen(file_name, "wb");

    tree_write_in_file(ctree, ctree->root_index, catalog, 0);
    fclose(catalog);
}


void catalog_add_feature(Catalog_tree *ctree, long long index, const char *new_node, const char *feature)
{
    size_t last_node     = ctree->tree[index].length_from_beginning;
    size_t last_node_len = ctree->tree[index].message_length;

    size_t new_node_len = strlen(new_node);
    size_t feature_len  = strlen(feature);

    long long index_after = tree_parent(ctree, index);

    if (index_after == ABSENT)
    {
        text_write_in_buffer(ctree->catalog, feature, feature_len);
        tree_insert(ctree, &index_after, YES, ctree->catalog->length - feature_len - 1, feature_len - 1);

        text_write_in_buffer(ctree->catalog, new_node, new_node_len);
        tree_insert(ctree, &index_after, YES, ctree->catalog->length - new_node_len - 1, new_node_len - 1);
        index_after = ctree->tree[index_after].parent;

        tree_insert(ctree, &index_after, NO, last_node, last_node_len);

        return;
    }

    char answer = ctree->tree[index].way;

    text_write_in_buffer(ctree->catalog, feature, feature_len);
    tree_insert(ctree, &index_after, answer, ctree->catalog->length - feature_len - 1, feature_len - 1);

    text_write_in_buffer(ctree->catalog, new_node, new_node_len);
    tree_insert(ctree, &index_after, YES, ctree->catalog->length - new_node_len - 1, new_node_len - 1);

    index_after = tree_parent(ctree, index_after);

    tree_insert(ctree, &index_after, NO, last_node, last_node_len);
}

void catalog_print_definition(Catalog_tree *ctree, long long place, long long from)
{
    Stack* stack_path = stack_new(ctree->size);

    long long path_len = (long long)catalog_count_path(ctree, stack_path, place) - 1;
    place = ctree->root_index;
    
    double way = 0;
    while (path_len >= 0)
    {
        stack_pop(&stack_path, &way);
        if ((int)way == NO)
        {
            catalog_linking_words(NEGATION, END_OF_WORD);
            catalog_main_words(ctree, place, LINKING_SENTENCE);
            place = ctree->tree[place].left_son;
        }
        else
        {
            catalog_main_words(ctree, place, LINKING_SENTENCE);
            place = ctree->tree[place].right_son;
        }

        path_len--;
    }

    if (ctree->tree[from].way == NO)
        catalog_linking_words(NEGATION, END_OF_WORD);
    catalog_main_words(ctree, ctree->tree[from].parent, END_OF_SENTENCE);

    stack_destruct(&stack_path);
}

void catalog_make_definition(Catalog_tree *ctree, const char *node)
{
    long long place = tree_search(ctree, node);
    if (place == ABSENT)
    {
        catalog_inform_user(NO_CHARACTER);
        return;
    }

    catalog_main_words(ctree, place, BEGIN_OF_SENTENCE);
    catalog_print_definition(ctree, ctree->tree[place].parent, place);
}

long long catalog_count_path(Catalog_tree* ctree, Stack *stack_path, long long place)
{
    long long path = 0;

    while (ctree->tree[place].parent != ABSENT)
    {
        stack_push(&stack_path, (double)ctree->tree[place].way);
        place = ctree->tree[place].parent;
        path++;
    }

    return path;
}

char catalog_print_feature(Catalog_tree* ctree, Stack *stack_path, long long* index, long long node_path)
{
    char no_negation = YES;

    if (!tree_if_lief(ctree, *index) && node_path) 
    {
        double future_way = 0;
        stack_earlier_back(&stack_path, &future_way);

        double current_way = 0;
        stack_back(&stack_path, &current_way);
        
        catalog_main_words(ctree, *index, LINKING_SENTENCE);
        if ((char)future_way == NO)
            no_negation = NO;
        if ((char)current_way == NO)
            *index = ctree->tree[*index].left_son;
        else
            *index = ctree->tree[*index].right_son;
    }
    else
    {
        catalog_main_words(ctree, *index, MIDDLE_OF_SENTENCE);
    }

    return no_negation;
}

void catalog_print_branch(Catalog_tree* ctree, Stack *stack_path, long long index, long long node_path)
{
    long long the_path = node_path;
    double current_way = 0;
    if ((the_path))
    {
        stack_back(&stack_path, &current_way);

        if ((char)current_way == NO)
            catalog_linking_words(NEGATION, END_OF_WORD);
    }

    char no_negation = YES;

    while (the_path >= 0)
    {
        if (no_negation == NO)
            catalog_linking_words(NEGATION, END_OF_WORD);
        no_negation = catalog_print_feature(ctree, stack_path, &index, the_path);

        stack_pop(&stack_path, &current_way);
        the_path--;
    }
}

void tree_print_difference(Catalog_tree* ctree, Stack *stack_path, long long* path_len, long long* index)
{
    double current_way = 0;
    stack_pop(&stack_path, &current_way);
    if ((int)current_way == NO)
    {
        catalog_linking_words(NEGATION, END_OF_WORD);
        *index = ctree->tree[*index].left_son;
    }
    else
        *index = ctree->tree[*index].right_son;
    
    if (!tree_if_lief(ctree, *index))
        catalog_main_words(ctree, ctree->tree[*index].parent, LINKING_SENTENCE);
    else
        catalog_main_words(ctree, ctree->tree[*index].parent, END_OF_SENTENCE);
    (*path_len)--;
}

void catalog_make_comparison(Catalog_tree* ctree, const char* first_node, const char* second_node)
{
    long long first_place  = tree_search(ctree, first_node);
    long long second_place = tree_search(ctree, second_node);
    if (first_place == ABSENT || second_place == ABSENT)
    {
        catalog_inform_user(NO_CHARACTER);
        return;
    }
//заменить на два стека
    Stack *first_path = stack_new(ctree->size);
    Stack *second_path = stack_new(ctree->size);
    //int* pathes = (int*)calloc(ctree->size * 2, sizeof(int));

    long long first_path_len  = catalog_count_path(ctree, first_path, first_place) - 1;
    long long second_path_len = catalog_count_path(ctree, second_path, second_place) - 1;

    long long index = ctree->root_index;

    double way_of_first = 0;
    double way_of_second = 0;

    stack_back(&first_path, &way_of_first);
    stack_back(&second_path, &way_of_second);
    if (way_of_first == way_of_second)
    {
        catalog_linking_words(GENERALIZATION, END_OF_WORD);
        if (way_of_first == NO)
            catalog_linking_words(NEGATION, END_OF_WORD);
    }

    char no_negation = YES;

    while (first_path_len && second_path_len && way_of_first == way_of_second)
    {
        if (no_negation == NO)
            catalog_linking_words(NEGATION, END_OF_WORD);
        
        no_negation = catalog_print_feature(ctree, first_path, &index, first_path_len);

        stack_pop(&first_path, &way_of_first);
        stack_pop(&second_path, &way_of_second);

        stack_back(&first_path, &way_of_first);
        stack_back(&second_path, &way_of_second);

        first_path_len--;
        second_path_len--;
    }

    long long branch_index = index;
    if (first_path_len >= 0 || second_path_len >= 0)
    {
        if (index != ctree->root_index)
            catalog_linking_words(EXEPTION, END_OF_WORD);
        if (first_path_len >= 0)
        {
            catalog_main_words(ctree, first_place, MIDDLE_OF_SENTENCE);
            tree_print_difference(ctree, first_path, &first_path_len, &index);

            catalog_print_branch(ctree, first_path, index, first_path_len);
            catalog_linking_words(ADDING, END_OF_WORD);
        }

        if (second_path_len >= 0)
        {
            catalog_main_words(ctree, second_place, MIDDLE_OF_SENTENCE);
            tree_print_difference(ctree, second_path, &second_path_len, &branch_index);

            catalog_print_branch(ctree, second_path, branch_index, second_path_len);
        }
        catalog_linking_words(END_OF_WORD, END_OF_STRING);
    }
    else catalog_print_feature(ctree, first_path, &index, first_path_len);

    stack_destruct(&first_path);
    stack_destruct(&second_path);
}

