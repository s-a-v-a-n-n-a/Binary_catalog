#include <stdio.h>

#define TX_USE_SPEAK
#include "TXLib.h"

#include "Stack.h"

#include "catalog_tree_consts.h"
#include "catalog_tree_work_with_text.h"
#include "catalog_tree.h"


void             catalog_inform_user                  (const char* information);
void             catalog_ask_user                     (Catalog_tree* ctree, long long index, const char* question);
void             catalog_linking_words                (const char* word, const char* mark);
void             catalog_main_words                   (Catalog_tree* ctree, long long index, char status_end);

static void      catalog_print_message                (Catalog_tree* ctree, long long index);
static void      catalog_print_phrase                 (const char* phrase, const char* beginning, const char *ending);
static void      catalog_print_message                (Catalog_tree* ctree, long long index, const char* before, const char* after);

static void      catalog_say_phrase                   (const char* phrase);
static void      catalog_say_message                  (Catalog_tree* ctree, long long index, const char* before, const char* after);

void             catalog_print_help                   ();

void             catalog_read_requests                (Catalog_tree* ctree);

void             catalog_call_picture                 (Catalog_tree *ctree);

int              catalog_check_character_identity     (Catalog_tree* ctree, long long index, const char* character);
void             catalog_add_new_character            (Catalog_tree *ctree, long long index, const char *character);

char             catalog_answer                       ();
void             catalog_game                         (Catalog_tree *ctree, long long index);

void             catalog_add_feature                  (Catalog_tree* ctree, long long index, const char* new_node, const char* feature);

long long        catalog_count_path                   (Catalog_tree* ctree, Stack *stack_path, long long place);

void             catalog_print_definition             (Catalog_tree* ctree, const long long place, long long from);

void             catalog_make_definition              (Catalog_tree* ctree, const char* node);

char             catalog_print_feature                (Catalog_tree* ctree, Stack* stack_path, long long* index, long long node_path);
void             catalog_print_branch                 (Catalog_tree* ctree, Stack* stack_path, long long index, long long node_path);

void             catalog_make_comparison              (Catalog_tree* ctree, const char* first_node, const char* second_node);

void             catalog_fprint_new                   (Catalog_tree* ctree, const char* file_name);
