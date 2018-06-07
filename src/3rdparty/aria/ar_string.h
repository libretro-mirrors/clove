/* Copyright (c) 2018 Murii
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license. See LICENSE for details.
 */
#ifndef AR_STRING_H
#define AR_STRING_H

#include "aria.h"

ar_Value *p_readchar(ar_State *S, ar_Value *args, ar_Value *env);
ar_Value *p_strcmp(ar_State *S, ar_Value *args, ar_Value *env);
ar_Value *p_substr(ar_State *S, ar_Value *args, ar_Value *env);
ar_Value *p_strpos(ar_State *S, ar_Value *args, ar_Value *env);
ar_Value *f_string(ar_State *S, ar_Value *args);
ar_Value *f_strlen(ar_State *S, ar_Value *args);
ar_Value *f_ord(ar_State *S, ar_Value *args);
ar_Value *f_chr(ar_State *S, ar_Value *args);

#endif

