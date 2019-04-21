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
    double alpha = fh_optnumber(args[3], 255.0);

    float scale = 1.0f / 255.0f;

    graphics_setBackgroundColor((float)red * scale, (float)green * scale, (float) blue * scale, (float) alpha * scale);
    return 0;
}

static int fn_love_graphics_getBackgroundColor(struct fh_program *prog,
                                               struct fh_value *ret, struct fh_value *args, int n_args) {
    float *colors = graphics_getBackgroundColor();

    struct fh_value arr_value = fh_new_array(prog);
    fh_grow_array(prog, &arr_value, 4);

    struct fh_array *arr = GET_VAL_ARRAY(&arr_value);

    for (int i = 0; i < 4; i++)
        arr->items[i] = fh_new_number((double)colors[i] * 255.0);

    *ret = arr_value;
    return 0;
}

static int fn_love_graphics_setColor(struct fh_program *prog,
                                     struct fh_value *ret, struct fh_value *args, int n_args) {
    if (!fh_is_number(&args[0]))
        return fh_set_error(prog, "Expected number");

    double red = fh_get_number(&args[0]);
    double green = fh_optnumber(args[1], red);
    double blue = fh_optnumber(args[2], green);
    double alpha = fh_optnumber(args[3], 255.0);

    float scale = 1.0f / 255.0f;

    graphics_setColor((float)red * scale, (float)green * scale, (float)blue * scale, (float)alpha * scale);
    return 0;
}

static int fn_love_graphics_getColor(struct fh_program *prog,
                                     struct fh_value *ret, struct fh_value *args, int n_args) {
    float *colors = graphics_getColor();

    struct fh_value arr_value = fh_new_array(prog);
    fh_grow_array(prog, &arr_value, 4);

    struct fh_array *arr = GET_VAL_ARRAY(&arr_value);

    for (int i = 0; i < 4; i++)
        arr->items[i] = fh_new_number((double)colors[i] * 255.0);

    *ret = arr_value;
    return 0;
}

static int fn_love_graphics_clear(struct fh_program *prog,
                                  struct fh_value *ret, struct fh_value *args, int n_args) {
    graphics_clear();
    return 0;
}

static int fn_love_graphics_push(struct fh_program *prog,
                                 struct fh_value *ret, struct fh_value *args, int n_args) {
    if(matrixstack_push()) {
        return fh_set_error(prog, "Matrix stack overflow\n");
    }
    return 0;
}

static int fn_love_graphics_pop(struct fh_program *prog,
                                struct fh_value *ret, struct fh_value *args, int n_args) {
    if(matrixstack_pop()) {
        return fh_set_error(prog, "Matrix stack underrun\n");
    }
    return 0;
}

static int fn_love_graphics_translate(struct fh_program *prog,
                                struct fh_value *ret, struct fh_value *args, int n_args) {
    if (!fh_is_number(&args[0]) || !fh_is_number(&args[1]))
        return fh_set_error(prog, "Expected at least 2 numbers as arguments\n");

    float x = (float)fh_get_number(&args[0]);
    float y = (float)fh_get_number(&args[1]);
    float z = (float)fh_optnumber(args[2], 1.0);

    matrixstack_translate(x, y, z);
    return 0;
}

static int fn_love_graphics_scale(struct fh_program *prog,
                                struct fh_value *ret, struct fh_value *args, int n_args) {
    if (!fh_is_number(&args[0]) || !fh_is_number(&args[1]))
        return fh_set_error(prog, "Expected at least 2 numbers as arguments\n");

    float x = (float)fh_get_number(&args[0]);
    float y = (float)fh_get_number(&args[1]);
    float z = (float)fh_optnumber(args[2], 1.0);

    matrixstack_scale(x, y, z);
    return 0;
}


static int fn_love_graphics_rotate(struct fh_program *prog,
                                struct fh_value *ret, struct fh_value *args, int n_args) {
    if (!fh_is_number(&args[0]) )
        return fh_set_error(prog, "Expected number as argument\n");

    float a = (float)fh_get_number(&args[0]);

    matrixstack_rotate(a);
    return 0;
}

static int fn_love_graphics_origin(struct fh_program *prog,
                                  struct fh_value *ret, struct fh_value *args, int n_args) {
    matrixstack_origin();
    return 0;
}

static int fn_love_graphics_shear(struct fh_program *prog,
                                  struct fh_value *ret, struct fh_value *args, int n_args) {
    return fh_set_error(prog, "not implemented");
}

static int fn_love_graphics_setColorMask(struct fh_program *prog,
                                  struct fh_value *ret, struct fh_value *args, int n_args) {
    if (fh_is_null(&args[0])) {
        graphics_setColorMask(true, true, true, true);
        return 0;
    } else {
        for (int i = 0; i < 4; ++i) {
            if (!fh_is_bool(&args[i])) {
                return fh_set_error(prog, "illegal parameters");
            }
        }
    }

    bool r = fh_get_bool(&args[0]);
    bool g = fh_get_bool(&args[1]);
    bool b = fh_get_bool(&args[2]);
    bool a = fh_get_bool(&args[3]);

    graphics_setColorMask(r, g, b, a);

    return 0;
}

static int fn_love_graphics_getColorMask(struct fh_program *prog,
                                  struct fh_value *ret, struct fh_value *args, int n_args) {
    bool r, g, b, a;
    graphics_getColorMask(&r, &g, &b, &a);

    struct fh_value arr_val = fh_new_array(prog);
    fh_grow_array(prog, &arr_val, 4);

    struct fh_array *arr = GET_VAL_ARRAY(&arr_val);

    arr->items[0] = fh_new_bool(r);
    arr->items[1] = fh_new_bool(g);
    arr->items[2] = fh_new_bool(b);
    arr->items[3] = fh_new_bool(a);

    return 0;
}

static int fn_love_graphics_getWidth(struct fh_program *prog,
                                  struct fh_value *ret, struct fh_value *args, int n_args) {
    *ret = fh_new_number(graphics_getWidth());
    return 0;
}

static int fn_love_graphics_getHeight(struct fh_program *prog,
                                  struct fh_value *ret, struct fh_value *args, int n_args) {
    *ret = fh_new_number(graphics_getHeight());
    return 0;
}

static int fn_love_graphics_reset(struct fh_program *prog,
                                  struct fh_value *ret, struct fh_value *args, int n_args) {
    graphics_reset();
    return 0;
}


/* TODO
static const l_tools_Enum l_graphics_BlendMode[] = {
    {"additive",       graphics_BlendMode_additive},
    {"alpha",          graphics_BlendMode_alpha},
    {"subtractive",    graphics_BlendMode_subtractive},
    {"multiplicative", graphics_BlendMode_multiplicative},
    {"premultiplied",  graphics_BlendMode_premultiplied},
    {"replace",        graphics_BlendMode_replace},
    {"screen",         graphics_BlendMode_screen},
    {NULL, 0}
};

static int l_graphics_setBlendMode(lua_State* state) {
    graphics_BlendMode mode = l_tools_toEnumOrError(state, 1, l_graphics_BlendMode);
    graphics_setBlendMode(mode);
    return 0;
}

static int l_graphics_getBlendMode(lua_State* state) {
    l_tools_pushEnum(state, graphics_getBlendMode(), l_graphics_BlendMode);
    return 1;
}

static int l_graphics_setScissor(lua_State* state) {
    if(lua_isnone(state, 1)) {
        graphics_clearScissor();
        return 0;
    } else {
        for(int i = 2; i < 5; ++i) {
            if(lua_isnone(state, i)) {
                lua_pushstring(state, "illegal paramters");
                return lua_error(state);
            }
        }
    }

    int x = l_tools_toNumberOrError(state, 1);
    int y = l_tools_toNumberOrError(state, 2);
    int w = l_tools_toNumberOrError(state, 3);
    int h = l_tools_toNumberOrError(state, 4);

    graphics_setScissor(x,y,w,h);

    return 0;
}

static int l_graphics_getScissor(lua_State* state) {
    int x,y,w,h;
    bool scissor = graphics_getScissor(&x, &y, &w, &h);
    if(!scissor) {
        return 0;
    }

    lua_pushinteger(state, x);
    lua_pushinteger(state, y);
    lua_pushinteger(state, w);
    lua_pushinteger(state, h);

    return 4;
}
*/

static const graphics_Quad defaultQuad = {
    .x = 0.0f,
    .y = 0.0f,
    .w = 1.0f,
    .h = 1.0f
};

static int fn_love_graphics_draw(struct fh_program *prog,
                                 struct fh_value *ret, struct fh_value *args, int n_args) {

    if (!fh_is_c_obj(&args[0]) || n_args < 3)
        return fh_set_error(prog, "Expected image, batch, mesh or particle and x, y coordonate");

    struct fh_c_obj *o = fh_get_c_obj(&args[0]);

    double x = fh_get_number_err(prog, args, 1);
    double y = fh_get_number_err(prog, args, 2);
    double r = fh_optnumber(args[3], 0);
    double sx = fh_optnumber(args[4], 1);
    double sy = fh_optnumber(args[5], sx);
    double ox = fh_optnumber(args[6], 0);
    double oy = fh_optnumber(args[7], ox);
    double kx = fh_optnumber(args[8], 0);
    double ky = fh_optnumber(args[9], kx);

    if (o->type == FH_IMAGE_TYPE) {
        fh_image_t *image = o->ptr;
        graphics_Image_draw(image->img, &defaultQuad, x, y, r, sx, sy, ox, oy, kx, ky);
    } else
        return fh_set_error(prog, "Expected image, batch, mesh or particle as argument 1");

    return 0;
}

#define DEF_FN(name) { #name, fn_##name }
static const struct fh_named_c_func c_funcs[] = {
    DEF_FN(love_graphics_setBackgroundColor),
    DEF_FN(love_graphics_getBackgroundColor),
    DEF_FN(love_graphics_setColor),
    DEF_FN(love_graphics_getColor),
    DEF_FN(love_graphics_push),
    DEF_FN(love_graphics_pop),
    DEF_FN(love_graphics_clear),
    DEF_FN(love_graphics_draw),
    DEF_FN(love_graphics_getWidth),
    DEF_FN(love_graphics_getHeight),
    DEF_FN(love_graphics_reset),
    DEF_FN(love_graphics_setColorMask),
    DEF_FN(love_graphics_getColorMask),
    DEF_FN(love_graphics_shear),
    DEF_FN(love_graphics_origin),
    DEF_FN(love_graphics_rotate),
    DEF_FN(love_graphics_scale),
    DEF_FN(love_graphics_translate),
};
void fh_graphics_register(struct fh_program *prog) {
    fh_add_c_funcs(prog, c_funcs, sizeof(c_funcs)/sizeof(c_funcs[0]));
}
