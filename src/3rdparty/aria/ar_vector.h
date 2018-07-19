/* Copyright (c) 2018 Murii
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license. See LICENSE for details.
 */
#ifndef AR_VECTOR_H
#define AR_VECTOR_H

#include "aria.h"

ar_Value *f_vectorp(ar_State *S, ar_Value *args);

ar_Value *p_vector(ar_State *S, ar_Value *args, ar_Value *env);
ar_Value *p_vector_push(ar_State *S, ar_Value *args, ar_Value *env);
ar_Value *p_vector_set(ar_State *S, ar_Value *args, ar_Value *env);
ar_Value *p_vector_remove(ar_State *S, ar_Value *args, ar_Value *env);
ar_Value *p_vector_pop(ar_State *S, ar_Value *args, ar_Value *env);
ar_Value *p_vector_get(ar_State *S, ar_Value *args, ar_Value *env);
ar_Value *p_vector_length(ar_State *S, ar_Value *args, ar_Value *env);
ar_Value *p_vector_member(ar_State *S, ar_Value *args, ar_Value *env);
ar_Value *p_vector_find(ar_State *S, ar_Value *args, ar_Value *env);
#endif

