#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <assert.h>

#include "catalog_tree_consts.h"

typedef struct file_text
{
    char  *buffer;
    //indicator of end of filled part
    size_t length;
    size_t capacity;
} Catalog;


Catalog   *text_init            (FILE *input);
void       text_delete          (Catalog *base);
void       text_construct       (FILE *input, Catalog *base);
void       text_destruct        (Catalog *base);

void       text_resize          (Catalog *base);

void       text_write_in_buffer (Catalog *base, const char *information, size_t information_length);

size_t     text_get_file_size   (FILE *file);
char      *text_get             (FILE *file, size_t *length);

FILE      *text_read            (const char *file_name);
