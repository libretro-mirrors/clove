#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vector.h"

void vector_init(vec_t *v)
{
    v->data = NULL;
    v->size = 0;
    v->count = 0;
}

int vector_count(vec_t *v)
{
    return v->count;
}

void vector_add(vec_t *v, void *e)
{
    if (v->size == 0) {
        v->size = 10;
        v->data = malloc(sizeof(void*) * v->size);
        memset(v->data, '\0', sizeof(void*) * v->size);
    }

    if (v->size == v->count) {
        v->size *= 2;
        v->data = realloc(v->data, sizeof(void*) * v->size);
    }

    v->data[v->count] = e;
    v->count++;
}

void vector_set(vec_t *v, int index, void *e)
{
    if (index >= v->count) {
        return;
    }

    v->data[index] = e;
}

void *vector_get(vec_t *v, int index)
{
    if (index >= v->count) {
        return NULL;
    }

    return v->data[index];
}

void vector_delete(vec_t *v, int index)
{
    if (index >= v->count) {
        return;
    }
    int i = index + 1;
    int j = index;
    while (i < v->count) {
        v->data[j] = v->data[i];
        ++i;
        ++j;
    }
    v->count--;
}
void vector_free(vec_t *v)
{
    free(v->data);
}

