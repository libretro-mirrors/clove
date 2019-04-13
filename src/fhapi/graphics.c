/*
#   clove
#
#   Copyright (C) 2019 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/

#include "graphics.h"

#include "../3rdparty/FH/src/value.h"
#include "../3rdparty/CMath/cmath.h"

#include "image.h"

#include "../include/graphics.h"
#include "../include/matrixstack.h"
#include "../include/shader.h"

static int fn_love_graphics_setBackgroundColor(struct fh_program *prog,
                                 struct fh_value *ret, struct fh_value *args, int n_args) {
    double red = fh_get_number_err(prog, args, 0);
    if (red == -1 && !fh_running)
        return -1;

    double green = fh_optnumber(args[1], red);
    double blue = fh_optnumber(args[2], green);
    double alpha = fh_optnumber(args[3], 255.0f);

    float scale = 1.0f / 255.0f;

    graphics_setBackgroundColor(red * scale, green * scale, blue * scale, alpha * scale);
    return 0;
}

static const graphics_Quad defaultQuad = {
    .x = 0.0f,
    .y = 0.0f,
    .w = 1.0f,
    .h = 1.0f
};

static int fn_love_graphics_drawImage(struct fh_program *prog,
                                 struct fh_value *ret, struct fh_value *args, int n_args) {

    fh_image_t *img = fh_get_c_obj(&args[0]);

    double x = fh_get_number_err(prog, args, 1);
    double y = fh_get_number_err(prog, args, 2);
    double r = fh_get_number(&args[3]);

    graphics_Image_draw(img->img, &defaultQuad, x, y, r, 1, 1, 0, 0, 0, 0);

    return 0;
}

#define DEF_FN(name) { #name, fn_##name }
static const struct fh_named_c_func c_funcs[] = {
    DEF_FN(love_graphics_setBackgroundColor),
    DEF_FN(love_graphics_drawImage),
};
void fh_graphics_register(struct fh_program *prog) {
    fh_add_c_funcs(prog, c_funcs, sizeof(c_funcs)/sizeof(c_funcs[0]));
}
