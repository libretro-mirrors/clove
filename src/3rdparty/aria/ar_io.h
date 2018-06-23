/* Copyright (c) 2018 Murii
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license. See LICENSE for details.
 */
#ifndef AR_IO_H
#define AR_IO_H

#include "aria.h"

ar_Value *f_read(ar_State *S, ar_Value *args);
ar_Value *f_write(ar_State *S, ar_Value *args);
ar_Value *p_import(ar_State *S, ar_Value *args, ar_Value *env);
ar_Value *p_free_import(ar_State *S, ar_Value *args, ar_Value *env);
ar_Value *p_open(ar_State *S, ar_Value *args, ar_Value *env);
ar_Value *p_dirp(ar_State *S, ar_Value *args, ar_Value *env);

#endif

