#include "src/catalog_tree_user_work.h"

int main(const int argc, const char* argv[])
{
    Catalog_tree *not_fake_akinator = NULL;
    if (argc > 1) not_fake_akinator = tree_read_from_file(argv[1]);
    else          not_fake_akinator = tree_read_from_file(DEFAULT_FILE_NAME);

    catalog_inform_user(GREETING);
    catalog_print_help   ();
    catalog_read_requests(not_fake_akinator);

    if (argc > 1) catalog_fprint_new(not_fake_akinator, argv[1]);
    else          catalog_fprint_new(not_fake_akinator, DEFAULT_FILE_NAME);
    tree_delete(not_fake_akinator);
    
    return 0;
}
