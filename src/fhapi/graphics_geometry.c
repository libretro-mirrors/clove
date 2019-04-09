/*
#   clove
#
#   Copyright (C) 2019 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/

#include "graphics_geometry.h"
#include "tools.h"

#include "../3rdparty/FH/src/fh.h"
#include "../3rdparty/FH/src/value.h"

#include "../include/geometry.h"

static int fn_love_geometry_circle(struct fh_program *prog,
        struct fh_value *ret, struct fh_value *args, int n_args) {

    const char* type = fh_tools_toStringOrError(prog, args, 0);
    float x = fh_tools_toNumberOrError(prog, args, 1);
    float y = fh_tools_toNumberOrError(prog, args, 2);
    float radius = fh_tools_toNumberOrError(prog, args, 3);
    float segments = fh_optnumber(args[4], 12);
    float rotation = fh_optnumber(args[5], 0);
    float sx = fh_optnumber(args[6], 1);
    float sy = fh_optnumber(args[7], sx);
    float ox = fh_optnumber(args[8], 0);
    float oy = fh_optnumber(args[9], ox);

    if (strcmp(type, "line") == 0)
        graphics_geometry_lineCircle(x, y, radius, segments, rotation, sx, sy, ox, oy);
    else if(strcmp(type, "fill") == 0)
        graphics_geometry_fillCircle(x, y, radius, segments, rotation, sx, sy, ox, oy);
    else {
        clove_running = false;
        return fh_print_error(prog, "Expected 'line' or 'fill' as input type!\n");
    }

    return 1;
}

static int fn_love_geometry_rectangle(struct fh_program *prog,
        struct fh_value *ret, struct fh_value *args, int n_args) {

    const char* type = fh_tools_toStringOrError(prog, args, 0);
    float x = fh_tools_toNumberOrError(prog, args, 1);
    float y = fh_tools_toNumberOrError(prog, args, 2);
    float w = fh_tools_toNumberOrError(prog, args, 3);
    float h = fh_optnumber(args[4], w);
    float rotation = fh_optnumber(args[5], 0);
    float sx = fh_optnumber(args[6], 1);
    float sy = fh_optnumber(args[7], sx);
    float ox = fh_optnumber(args[8], 0);
    float oy = fh_optnumber(args[9], ox);

    if (strcmp(type, "line") == 0)
        graphics_geometry_rectangle(false, x, y, w, h, rotation, sx, sy, ox, oy);
    else if(strcmp(type, "fill") == 0)
        graphics_geometry_rectangle(true, x, y, w, h, rotation, sx, sy, ox, oy);
    else {
        clove_running = false;
        return fh_print_error(prog, "Expected 'line' or 'fill' as input type!\n");
    }

    return 1;
}

#define DEF_FN(name) { #name, fn_##name }
static const struct fh_named_c_func c_funcs[] = {
    DEF_FN(love_geometry_circle),
    DEF_FN(love_geometry_rectangle),
};

void fh_graphics_geometry_register(struct fh_program *prog) {
    fh_add_c_funcs(prog, c_funcs, sizeof(c_funcs)/sizeof(c_funcs[0]));
}
