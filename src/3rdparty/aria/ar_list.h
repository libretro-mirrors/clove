/* Copyright (c) 2018 Murii
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license. See LICENSE for details.
 */
#ifndef AR_LIST_H
#define AR_LIST_H

#include "aria.h"

ar_Value *p_find(ar_State *S, ar_Value *args, ar_Value *env);
ar_Value *p_reverse(ar_State *S, ar_Value *args, ar_Value *env);
ar_Value *p_nth(ar_State *S, ar_Value *args, ar_Value *env);
ar_Value *p_nthcdr(ar_State *S, ar_Value *args, ar_Value *env);
ar_Value *p_remove(ar_State *S, ar_Value *args, ar_Value *env);
ar_Value *p_remove_n(ar_State *S, ar_Value *args, ar_Value *env);
ar_Value *p_length(ar_State *S, ar_Value *args, ar_Value *env);
ar_Value *p_member(ar_State *S, ar_Value *args, ar_Value *env);
ar_Value *p_append(ar_State *S, ar_Value *args, ar_Value *env);

ar_Value *f_list(ar_State *S, ar_Value *args);
ar_Value *f_cons(ar_State *S, ar_Value *args);
ar_Value *f_cdr(ar_State *S, ar_Value *args);
ar_Value *f_car(ar_State *S, ar_Value *args);
ar_Value *f_setcar(ar_State *S, ar_Value *args);
ar_Value *f_setcdr(ar_State *S, ar_Value *args);
ar_Value *f_consp(ar_State *S, ar_Value *args);

#endif

