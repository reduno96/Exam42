#ifndef ARGO_H
#define ARGO_H

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
// this file is given in the exam

typedef struct json
{
    enum
    {
        MAP,
        INTEGER,
        STRING
    } type;
    union
    {
        struct
        {
            struct pair *data;
            size_t size;
        } map;
        int integer;
        char *string;
    };
} json;

typedef struct pair
{
    char *key;
    json value;
} pair;

void free_json(json j);
int argo(json *dst, FILE *stream);
void unexpected(FILE *stream);
int peek(FILE *stream);

#endif