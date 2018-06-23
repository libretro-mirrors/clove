/* Copyright (c) 2018 Murii
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license. See LICENSE for details.
 */
#ifndef AR_MAP_H
#define AR_MAP_H

#include "aria.h"

ar_Value *p_map(ar_State *S, ar_Value *args, ar_Value *env);
ar_Value *p_map_add(ar_State *S, ar_Value *args, ar_Value *env);
ar_Value *p_map_get(ar_State *S, ar_Value *args, ar_Value *env);
ar_Value *p_map_remove(ar_State *S, ar_Value *args, ar_Value *env);

#endif

