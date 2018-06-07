/* Copyright (c) 2018 Murii
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license. See LICENSE for details.
 */
#ifndef AR_MATH_H
#define AR_MATH_H

#include "aria.h"

ar_Value *p_max(ar_State *S, ar_Value *args, ar_Value *env);
ar_Value *p_min(ar_State *S, ar_Value *args, ar_Value *env);
ar_Value *p_cos(ar_State *S, ar_Value *args, ar_Value *env);
ar_Value *p_sin(ar_State *S, ar_Value *args, ar_Value *env);
ar_Value *p_pi(ar_State *S, ar_Value *args, ar_Value *env);
ar_Value *p_floor(ar_State *S, ar_Value *args, ar_Value *env);
ar_Value *p_ceil(ar_State *S, ar_Value *args, ar_Value *env);
ar_Value *p_abs(ar_State *S, ar_Value *args, ar_Value *env);
ar_Value *p_sqrt(ar_State *S, ar_Value *args, ar_Value *env);
ar_Value *p_pow(ar_State *S, ar_Value *args, ar_Value *env);
ar_Value *p_random_seed(ar_State *S, ar_Value *args, ar_Value *env);
ar_Value *p_random(ar_State *S, ar_Value *args, ar_Value *env);
ar_Value *p_mod(ar_State *S, ar_Value *args, ar_Value *env);
ar_Value *p_add(ar_State *S, ar_Value *args, ar_Value *env);
ar_Value *p_sub(ar_State *S, ar_Value *args, ar_Value *env);
ar_Value *p_mul(ar_State *S, ar_Value *args, ar_Value *env);
ar_Value *p_div(ar_State *S, ar_Value *args, ar_Value *env);

#endif


