/*
#   clove
#
#   Copyright (C) 2019 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/
#pragma once

#ifdef USE_FH
#include "../3rdparty/FH/src/fh.h"
#include "../3rdparty/FH/src/value.h"

inline char const *fh_tools_toStringOrError(struct fh_program *prog, struct fh_value *args, int index)
{
    if (!fh_is_string(&args[index])){
        fh_set_error(prog, "Expected string value, got %s", fh_type_to_str(prog, args[index].type));
        return "";
    }
    return GET_VAL_STRING_DATA(&args[index]);
}
#endif
