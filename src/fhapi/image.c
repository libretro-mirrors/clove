/*
#   clove
#
#   Copyright (C) 2019 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/
#include "image.h"

#include <stdlib.h>
#include <stdio.h>

#include "../3rdparty/FH/src/value.h"

static void onFreeCallback(fh_image_t *data) {
    graphics_Image_free(data->img);
    image_ImageData_free(data->data);
    free(data->data);
    free(data->img);
    free(data);
}

static int fn_love_graphics_newImage(struct fh_program *prog,
                                     struct fh_value *ret, struct fh_value *args, int n_args) {
    if (n_args == 0)
        return fh_set_error(prog, "Expected path or width & height");

    fh_image_t *data = malloc(sizeof(fh_image_t));

    data->data = malloc(sizeof(image_ImageData));
    data->img = malloc(sizeof(graphics_Image));

    fh_c_obj_gc_callback onFree = onFreeCallback;

    enum fh_value_type type = args[0].type;
    if (type == FH_VAL_STRING) {
        const char *path = fh_get_string(&args[0]);
        image_ImageData_new_with_filename(data->data, path);
    }  else if (type == FH_VAL_FLOAT && args[1].type == FH_VAL_FLOAT) {
        image_ImageData_new_with_size(data->data, args[0].data.num, args[1].data.num, 3);
    } else {
        free(data->img);
        free(data->data);
        free(data);
        return fh_set_error(prog, "Expected path or width & height");
    }

    graphics_Image_new_with_ImageData(data->img, data->data);
    *ret = fh_new_c_obj(prog, data, onFree);

    return 0;
}

static int fn_love_image_getWidth(struct fh_program *prog,
                                     struct fh_value *ret, struct fh_value *args, int n_args) {

    image_ImageData *img = fh_get_c_obj(&args[0]);
    *ret = fh_new_number(image_ImageData_getWidth(img));
    return 0;
}

static int fn_love_image_getHeight(struct fh_program *prog,
                                     struct fh_value *ret, struct fh_value *args, int n_args) {

    image_ImageData *img = fh_get_c_obj(&args[0]);

    *ret = fh_new_number(image_ImageData_getHeight(img));
    return 0;
}

static int fn_love_image_getPath(struct fh_program *prog,
                                     struct fh_value *ret, struct fh_value *args, int n_args) {

    image_ImageData *img = fh_get_c_obj(&args[0]);

    *ret = fh_new_string(prog, image_ImageData_getPath(img));
    return 0;
}

static int fn_love_image_getChannels(struct fh_program *prog,
                                     struct fh_value *ret, struct fh_value *args, int n_args) {

    image_ImageData *img = fh_get_c_obj(&args[0]);

    *ret = fh_new_number(image_ImageData_getChannels(img));
    return 0;
}

#define DEF_FN(name) { #name, fn_##name }
static const struct fh_named_c_func c_funcs[] = {
    DEF_FN(love_graphics_newImage),
    DEF_FN(love_image_getWidth),
    DEF_FN(love_image_getHeight),
    DEF_FN(love_image_getPath),
    DEF_FN(love_image_getChannels),
};
void fh_image_register(struct fh_program *prog) {
    fh_add_c_funcs(prog, c_funcs, sizeof(c_funcs)/sizeof(c_funcs[0]));
}
