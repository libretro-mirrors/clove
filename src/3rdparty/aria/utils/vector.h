#ifndef VECTOR_H__
#define VECTOR_H__

typedef struct vector_ {
    void** data;
    int size;
    int count;
} vec_t;

void vector_init(vec_t*);
int vector_count(vec_t*);
void vector_add(vec_t*, void*);
void vector_set(vec_t*, int, void*);
void *vector_get(vec_t*, int);
void vector_delete(vec_t*, int);
void vector_free(vec_t*);

#endif
