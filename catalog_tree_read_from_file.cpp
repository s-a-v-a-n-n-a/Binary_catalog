#pragma once

#include "catalog_tree_read_from_file.h"


Catalog *text_init(FILE *input)
{
    Catalog *base = (Catalog*)calloc(1, sizeof(Catalog));
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

void text_delete(Catalog *base)
{
    text_destruct(base);

    free(base);
}

void text_construct(FILE *input, Catalog *base)
{
    base->length   = text_get_file_size(input);
    base->capacity = base->length;
    base->buffer   = text_get(input, &base->capacity);
}

void text_destruct(Catalog *base)
{
    free(base->buffer);
}

void text_resize(Catalog *base)
{
    size_t new_capacity = base->capacity * 2;

    char *new_buffer = (char*)realloc(base->buffer, new_capacity * sizeof(char));

    assert(new_buffer);

    base->buffer   = new_buffer;
    base->capacity = new_capacity;
}

void text_write_in_buffer(Catalog *base, const char *information, size_t information_length)
{
    if (base->length + information_length + 2 >= base->capacity)
        text_resize(base);

    //+1 чтоб следующий не скушал последний символ
    memcpy(base->buffer + base->length - 1, information, information_length + 1);

    base->length += information_length;
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

FILE *text_read(const char *file_name)
{
    FILE* input = fopen(file_name, "rb");
    if (!input)
        return NULL;

    return input;
}
