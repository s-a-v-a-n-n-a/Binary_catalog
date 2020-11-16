#pragma once

#include <stdio.h>
#include <stdlib.h>

#include "binary_tree_consts.h"

typedef struct file_text
{
    char  *buffer;
    size_t length;
} File_text;


File_text *text_init          (FILE *input);

void       text_delete        (File_text *base);

void       text_construct     (FILE *input, File_text *base);

void       text_destruct      (File_text *base);

size_t     text_get_file_size (FILE *file);

char      *text_get           (FILE *file, size_t *length);

File_text *text_read          (const char *file_name);
