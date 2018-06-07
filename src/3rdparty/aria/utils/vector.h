/* Copyright (c) 2014 Christopher Swenson.
 * Copyright (c) 2017-2018 Muresan Vlad Mihail
 */

/*
 * Thanks to aerx and MasterGeek_ for pointing out and solving the issue if capacity is 1
 * Capacity is set to a hard coded value of 8 because after doing tests that seems to be the best optimal value performance wise
 */

#ifndef __VECTOR_H
#define __VECTOR_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct {
    void** arr;
    size_t size;
    size_t capacity;
} vec_t;

/* type: char*,int,short,float, etc */
static __inline vec_t* vec_init(size_t type_size)
{
    vec_t* vec;
    vec = malloc(sizeof(vec_t));

    vec->capacity = 8;
    vec->arr = malloc(type_size * vec->capacity);
    vec->size = 0;

    return vec;
}

static __inline void vec_append(vec_t* vec, void* elt)
{
    if (vec->size == vec->capacity)
    {
        vec->capacity += vec->capacity | 1; /* Increase capacity by 100% and be sure it's >= than 1 */
        vec->arr = realloc(vec->arr, vec->capacity * sizeof(elt));
        if (!vec->arr)
            printf("Error: Could not realloc in vec_append! \n");
    }

    vec->arr[vec->size++] = elt;
}

static __inline int vec_set(vec_t* vec, size_t index, void* value)
{
	if (index > vec->size - 1)
		return 1;

	vec->arr[index] = value;
	return 0;
}

static __inline void* vec_get(vec_t* vec, size_t index)
{
	if (index > vec->size - 1)
		return NULL;

	return vec->arr[index];
}

static __inline void* vec_pop(vec_t* vec)
{
    return vec->arr[vec->size--];
}

static __inline int vec_del(vec_t* vec, size_t index, size_t type_size)
{
	if (index > vec->size)
		return 1;

    if (index == vec->size) {
        vec_pop(vec);
    } else {
        memmove(&(vec->arr[index]), &(vec->arr[index + 1]),
                type_size * vec->size - index - 1);
        vec->size--;
    }
	return 0;
}

static __inline size_t vec_size(vec_t* vec)
{
    return vec->size;
}

static __inline void vec_destroy(vec_t* vec)
{
    free(vec->arr);
    free(vec);
}

#undef VECTOR_TYPE
#endif
