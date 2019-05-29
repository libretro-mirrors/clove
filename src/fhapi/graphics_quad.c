/*
#   clove
#
#   Copyright (C) 2019 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/
#include "graphics_quad.h"

#include <stdlib.h>

#include "../3rdparty/FH/src/value.h"

#include "../include/quad.h"

static fh_c_obj_gc_callback quad_gc(graphics_Quad *quad) {
    free(quad);
    return (fh_c_obj_gc_callback)1;
}

static int fn_love_graphics_newQuad(struct fh_program *prog,
                                    struct fh_value *ret, struct fh_value *args, int n_args) {

    if (n_args != 6) {
        return fh_set_error(prog, "Expected 6 arguments!");
    }

    for (int i = 0; i < 6; i++) {
        if (!fh_is_number(&args[i]))
            return fh_set_error(prog, "Argument %d must be a number", i);
    }

    float x = (float)fh_get_number(&args[0]);
    float y = (float)fh_get_number(&args[1]);
    float w = (float)fh_get_number(&args[2]);
    float h = (float)fh_get_number(&args[3]);
    float rw = (float)fh_get_number(&args[4]);
    float rh = (float)fh_get_number(&args[5]);

    graphics_Quad *quad = malloc(sizeof(graphics_Quad));
    graphics_Quad_newWithRef(quad, x, y, w, h, rw, rh);

    *ret = fh_new_c_obj(prog, quad, quad_gc, FH_GRAPHICS_QUAD);
    return 0;
}

static int fn_love_graphics_getViewport(struct fh_program *prog,
                                    struct fh_value *ret, struct fh_value *args, int n_args) {
    if (fh_is_c_obj_of_type(&args[0], FH_GRAPHICS_QUAD)) {
        return fh_set_error(prog, "Expected quad object as first argument!");
    }

    graphics_Quad *quad = fh_get_c_obj_value(&args[0]);

    struct fh_value arr_val = fh_new_array(prog);
    fh_grow_array(prog, &arr_val, 4);

    struct fh_array *arr = GET_VAL_ARRAY(&arr_val);

    arr->items[0] = fh_new_number(quad->x);
    arr->items[1] = fh_new_number(quad->y);
    arr->items[2] = fh_new_number(quad->w);
    arr->items[3] = fh_new_number(quad->h);

    *ret = arr_val;
    return 0;
}

static int fn_love_graphics_setViewport(struct fh_program *prog,
                                    struct fh_value *ret, struct fh_value *args, int n_args) {

    if (n_args != 5)
        return fh_set_error(prog, "Expected 5 arguemnts!");

    if (fh_is_c_obj_of_type(&args[0], FH_GRAPHICS_QUAD)) {
        return fh_set_error(prog, "Expected quad object as first argument!");
    }

    for (int i = 1; i < 5; i++) {
        if (!fh_is_number(&args[i]))
            return fh_set_error(prog, "Expected number as argument %d", i);
    }

    float x = (float)fh_get_number(&args[0]);
    float y = (float)fh_get_number(&args[1]);
    float w = (float)fh_get_number(&args[2]);
    float h = (float)fh_get_number(&args[3]);

    graphics_Quad *quad = fh_get_c_obj_value(&args[0]);
    quad->x = x;
    quad->y = y;
    quad->w = w;
    quad->h = h;

    *ret = fh_new_null();
    return 0;
}

#define DEF_FN(name) { #name, fn_##name }
static const struct fh_named_c_func c_funcs[] = {
    DEF_FN(love_graphics_newQuad),
    DEF_FN(love_graphics_getViewport),
    DEF_FN(love_graphics_setViewport)
};

void fh_graphics_quad_register(struct fh_program *prog) {
    fh_add_c_funcs(prog, c_funcs, sizeof(c_funcs)/sizeof(c_funcs[0]));
}
