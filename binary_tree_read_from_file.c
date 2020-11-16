#pragma once

#include <assert.h>

#include "binary_tree_read_from_file.h"


File_text *text_init(FILE *input)
{
    File_text *base = (file_text*)calloc(1, sizeof(file_text));
    if (!base)
    {
        return NULL;
    }

    text_construct(input, base);
    if (!base->buffer)
    {
        text_destruct(base);

        return NULL;
    }

    return base;
}

void text_delete(File_text *base)
{
    text_destruct(base);

    free(base);
}

void text_construct(FILE *input, File_text *base)
{
    base->length = text_get_file_size(input);
    base->buffer = text_get          (input, &base->length);
}

void text_destruct(File_text *base)
{
    free(base->buffer);
}

size_t text_get_file_size (FILE *file)
{
    assert(file != NULL);

    long int length = 0;

    if (!fseek(file, 0, SEEK_END))
        length = ftell(file);
    length++;

    fseek(file, 0, SEEK_SET);

    return length;
}

char *text_get(FILE *file, size_t *length)
{
    assert(file != NULL && *length != 0);

    char *buffer = (char*) calloc(*length + 1, sizeof(char));
    if (!buffer)
    {
        return NULL;
    }
    long int obj = fread(buffer, sizeof(char), *length + 1, file);

    assert((size_t)obj == *length - 1);

    buffer[*length] = '\0';

    return buffer;
}

File_text *text_read(const char *file_name)
{
    FILE* input = fopen(file_name, "rb");
    if (!input)
        return NULL;

    File_text *base = text_init(input);

    fclose(input);

    return base;
}
