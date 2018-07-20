/* Copyright (c) 2018 Murii
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license. See LICENSE for details.
 */

#include <stdint.h>

#include "utils/vector.h"

#include "ar_vector.h"

ar_Value *f_vectorp(ar_State *S, ar_Value *args) {
    return ar_check(S, ar_car(args), AR_TVECTOR) ? S->t : NULL;
}


ar_Value *p_vector(ar_State *S, ar_Value *args, ar_Value *env) {
    UNUSED(S);

    vec_t *vector = malloc(sizeof(vec_t));
    vector_init(vector);

    while (ar_car(args) != NULL) {
        ar_Value *curr = ar_eval(S, ar_car(args), env);

        if (ar_type(args) == AR_TPAIR) {
            vector_add(vector, (ar_Value*)curr);
        }
        else {
            if (ar_type(curr) != AR_TVECTOR)
            {
                ar_Value *entry = NULL;
                while (curr != NULL) {
                    entry = ar_new_pair(S, curr, entry);
                    curr = ar_cdr(curr);
                }
                entry = ar_reverse(entry);
                vector_add(vector,  (ar_Value*) entry);
            }
            else
                vector_add(vector, (ar_Value*)curr);
        }


        args = ar_cdr(args);
    }

    return ar_new_vector(S, vector);
}


ar_Value *p_vector_push(ar_State *S, ar_Value *args, ar_Value *env) {
    vec_t *vector = ar_eval_vector(S, ar_car(args), env);
    if (!vector) {
        ar_error_str(S, "Missing vector value in vector-push");
    }
    ar_Value *to_push= ar_eval(S, ar_nth(args, 1), env);
    if (!to_push) {
        ar_error_str(S, "Missing value in vector-push");
    }
    if (ar_type(to_push) == AR_TVECTOR) {
        vector_add(vector, to_push);
    } else {
        ar_Value *entry = NULL;
        while (to_push) {
            entry = ar_new_pair(S, ar_car(to_push), entry);
            to_push = ar_cdr(to_push);
        }
        entry = ar_reverse(entry);
        vector_add(vector, entry);
    }

    return NULL;
}


ar_Value *p_vector_set(ar_State *S, ar_Value *args, ar_Value *env) {
    #ifdef AR_FLOAT
    float index;
    #else
    double index;
    #endif

    vec_t *vector = ar_eval_vector(S, ar_car(args), env);
    index = ar_eval_number(S, ar_nth(args, 1), env);
    if (index > vector_count(vector))
        ar_error_str(S, "index out of bounds");
    ar_Value *value = ar_eval(S, ar_nth(args, 2), env);
    vector_set(vector,index,ar_new_pair(S, value, NULL));
    return NULL;
}


ar_Value *p_vector_remove(ar_State *S, ar_Value *args, ar_Value *env) {
    vec_t *vector = ar_eval_vector(S, ar_car(args), env);
#ifdef AR_FLOAT
    float index;
#else
    double index;
#endif
    index = ar_eval_number(S, ar_nth(args, 1), env);
    vector_delete(vector, index);
    return NULL;
}

ar_Value *p_vector_pop(ar_State *S, ar_Value *args, ar_Value *env) {
    vec_t *vector = ar_eval_vector(S, ar_car(args), env);
    /*vec_pop(vector);*/
    return NULL;
}

ar_Value *p_vector_get(ar_State *S, ar_Value *args, ar_Value *env) {
    #ifdef AR_FLOAT
    float index;
    #else
    double index;
    #endif

    vec_t *vector = ar_eval_vector(S, ar_car(args), env);
    index = ar_eval_number(S, ar_nth(args, 1), env);
    ar_Value *ret = vector_get(vector, index);
    if (!ret)
        ar_error_str(S, "index out of bounds");
    return ar_car(ret);
}


ar_Value *p_vector_length(ar_State *S, ar_Value *args, ar_Value *env) {
    vec_t *vector = ar_eval_vector(S, ar_car(args), env);
    return ar_new_number(S, vector_count(vector));
}


ar_Value *p_vector_find(ar_State *S, ar_Value *args, ar_Value *env) {
    vec_t *from = ar_eval_vector(S, ar_car(args), env);
    #ifdef AR_FLOAT
    float index;
    #else
    double index;
    #endif

    index = ar_eval_number(S, ar_nth(args, 1), env);
    ar_Value *to_find = ar_eval(S, ar_nth(args, 2), env);

    while (index < vector_count(from)) {
     if (is_equal(ar_car(vector_get(from, index)), to_find) != 0)
            return ar_new_number(S, index);
     index++;
    }
    return NULL;
}

