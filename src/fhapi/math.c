/*
#   clove
#
#   Copyright (C) 2019 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/

#include "math.h"

#include "../3rdparty/noise/simplexnoise.h"


static int fn_love_math_noise(struct fh_program *prog,
                                  struct fh_value *ret, struct fh_value *args, int n_args)
{

    double v[4];

    if (n_args == 0 || n_args > 4)
        return fh_set_error(prog, "Illegal number of arguments, expected between 1 and 4");

    for (int i = 0; i < n_args; i++) {
        if (!fh_is_number(&args[i]))
            return fh_set_error(prog, "Expected number at index %d", i);
        v[i] = fh_get_number(&args[i]);
    }

    switch (n_args) {
    case 1:
        *ret = fh_new_number((float)simplexnoise_noise1(v[0]));
        break;
    case 2:
        *ret = fh_new_number((float)simplexnoise_noise2(v[0], v[1]));
        break;

    case 3:
        *ret = fh_new_number((float)simplexnoise_noise3(v[0], v[1], v[2]));
        break;

    case 4:
        *ret = fh_new_number((float)simplexnoise_noise4(v[0], v[1], v[2], v[3]));
        break;
    }
    return 0;
}


#define DEF_FN(name) { #name, fn_##name }
static const struct fh_named_c_func c_funcs[] = {
    DEF_FN(love_math_noise),
};

void fh_math_register(struct fh_program *prog) {
    fh_add_c_funcs(prog, c_funcs, sizeof(c_funcs)/sizeof(c_funcs[0]));
}
