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
    *ret = fh_new_c_obj(prog, data, onFree, FH_IMAGE_TYPE);
    return 0;
}

static int fn_love_image_getWidth(struct fh_program *prog,
                                  struct fh_value *ret, struct fh_value *args, int n_args) {
    if (!fh_is_c_obj_of_type(&args[0], FH_IMAGE_TYPE))
        return fh_set_error(prog, "Expected image");

    image_ImageData *img = fh_get_c_obj_value(&args[0]);
    *ret = fh_new_number(image_ImageData_getWidth(img));
    return 0;
}

static int fn_love_image_getHeight(struct fh_program *prog,
                                   struct fh_value *ret, struct fh_value *args, int n_args) {
    if (!fh_is_c_obj_of_type(&args[0], FH_IMAGE_TYPE))
        return fh_set_error(prog, "Expected image");

    image_ImageData *img = fh_get_c_obj_value(&args[0]);

    *ret = fh_new_number(image_ImageData_getHeight(img));
    return 0;
}

static int fn_love_image_getDimensions(struct fh_program *prog,
                                  struct fh_value *ret, struct fh_value *args, int n_args) {
    if (!fh_is_c_obj_of_type(&args[0], FH_IMAGE_TYPE))
        return fh_set_error(prog, "Expected image");

    fh_image_t *img = fh_get_c_obj_value(&args[0]);

    struct fh_value v = fh_new_array(prog);
    fh_grow_array(prog, &v, 2);

    struct fh_array *arr = GET_VAL_ARRAY(&v);
    arr->items[0] = fh_new_number(img->img->width);
    arr->items[1] = fh_new_number(img->img->height);

    *ret = v;
    return 0;
}

static int fn_love_image_getPath(struct fh_program *prog,
                                 struct fh_value *ret, struct fh_value *args, int n_args) {
    if (!fh_is_c_obj_of_type(&args[0], FH_IMAGE_TYPE))
        return fh_set_error(prog, "Expected image");

    image_ImageData *img = fh_get_c_obj_value(&args[0]);

    *ret = fh_new_string(prog, image_ImageData_getPath(img));
    return 0;
}

static int fn_love_image_getChannels(struct fh_program *prog,
                                     struct fh_value *ret, struct fh_value *args, int n_args) {
    if (!fh_is_c_obj_of_type(&args[0], FH_IMAGE_TYPE))
        return fh_set_error(prog, "Expected image");

    image_ImageData *img = fh_get_c_obj_value(&args[0]);

    *ret = fh_new_number(image_ImageData_getChannels(img));
    return 0;
}

static int fn_love_image_refresh(struct fh_program *prog,
                                 struct fh_value *ret, struct fh_value *args, int n_args) {
    if (!fh_is_c_obj_of_type(&args[0], FH_IMAGE_TYPE) || !fh_is_c_obj_of_type(&args[1], FH_IMAGE_TYPE))
        return fh_set_error(prog, "Expected two image data");

    fh_image_t *img = fh_get_c_obj_value(&args[0]);
    fh_image_t *imgd = fh_get_c_obj_value(&args[1]);

    graphics_Image_refresh(img->img, imgd->data);

    return 0;
}

static int fn_love_image_getWrap(struct fh_program *prog,
                                 struct fh_value *ret, struct fh_value *args, int n_args) {
    if (!fh_is_c_obj_of_type(&args[0], FH_IMAGE_TYPE))
        return fh_set_error(prog, "Expected image");

    fh_image_t *img = fh_get_c_obj_value(&args[0]);

    graphics_Wrap wrap;
    graphics_Image_getWrap(img->img, &wrap);

    struct fh_value v = fh_new_array(prog);
    fh_grow_array(prog, &v, 2);

    struct fh_array *arr = GET_VAL_ARRAY(&v);

    if (wrap.horMode == graphics_WrapMode_clamp)
        arr->items[0] = fh_new_string(prog, "clamp");
    else if (wrap.horMode == graphics_WrapMode_repeat) {
        arr->items[0] = fh_new_string(prog, "repeat");
    }

    if (wrap.verMode == graphics_WrapMode_clamp)
        arr->items[0] = fh_new_string(prog, "clamp");
    else if (wrap.verMode == graphics_WrapMode_repeat) {
        arr->items[0] = fh_new_string(prog, "repeat");
    }

    *ret = v;

    return 0;
}

static int fn_love_image_setWrap(struct fh_program *prog,
                                 struct fh_value *ret, struct fh_value *args, int n_args) {
    if (!fh_is_c_obj_of_type(&args[0], FH_IMAGE_TYPE) || !fh_is_string(&args[1]) || !fh_is_string(&args[2]))
        return fh_set_error(prog, "Expected image, horizontal and veritical string mode");

    fh_image_t *img = fh_get_c_obj_value(&args[0]);
    const char *horMode = fh_get_string(&args[1]);
    const char *verMode = fh_get_string(&args[2]);

    graphics_Wrap wrap;

    if (strcmp(horMode, "clamp") == 0) {
        wrap.horMode = graphics_WrapMode_clamp;
    } else if (strcmp(horMode, "repeat") == 0) {
        wrap.horMode = graphics_WrapMode_repeat;
    }

    if (strcmp(verMode, "clamp") == 0) {
        wrap.verMode = graphics_WrapMode_clamp;
    } else if (strcmp(verMode, "repeat") == 0) {
        wrap.verMode = graphics_WrapMode_repeat;
    }

    graphics_Image_setWrap(img->img, &wrap);

    return 0;
}

static int fn_love_image_getFilter(struct fh_program *prog,
                                 struct fh_value *ret, struct fh_value *args, int n_args) {
    if (!fh_is_c_obj_of_type(&args[0], FH_IMAGE_TYPE))
        return fh_set_error(prog, "Expected image");

    fh_image_t *img = fh_get_c_obj_value(&args[0]);

    graphics_Filter filter;
    graphics_Image_getFilter(img->img, &filter);

    struct fh_value v = fh_new_array(prog);
    fh_grow_array(prog, &v, 3);

    struct fh_array *arr = GET_VAL_ARRAY(&v);


    if (filter.minMode == graphics_FilterMode_none)
        arr->items[0] = fh_new_string(prog, "none");
    else if (filter.minMode == graphics_FilterMode_linear)
        arr->items[0] = fh_new_string(prog, "linear");
    else if (filter.minMode == graphics_FilterMode_nearest)
        arr->items[0] = fh_new_string(prog, "nearest");

    if (filter.magMode == graphics_FilterMode_none)
        arr->items[1] = fh_new_string(prog, "none");
    else if (filter.magMode == graphics_FilterMode_linear)
        arr->items[1] = fh_new_string(prog, "linear");
    else if (filter.magMode == graphics_FilterMode_nearest)
        arr->items[1] = fh_new_string(prog, "nearest");

    arr->items[2] = fh_new_number(filter.maxAnisotropy);

    *ret = v;

    return 0;
}

static int fn_love_image_setFilter(struct fh_program *prog,
                                 struct fh_value *ret, struct fh_value *args, int n_args) {
    if (!fh_is_c_obj_of_type(&args[0], FH_IMAGE_TYPE) || !fh_is_string(&args[1]) || !fh_is_string(&args[2]))
        return fh_set_error(prog, "Expected image, min and mag string mode");

    fh_image_t *img = fh_get_c_obj_value(&args[0]);
    const char *min = fh_get_string(&args[1]);
    const char *mag = fh_get_string(&args[2]);

    graphics_Filter newFilter;
    newFilter.maxAnisotropy = (float)fh_optnumber(args, n_args, 3, 1.0);

    if (strcmp(min, "none") == 0) {
        newFilter.minMode = graphics_FilterMode_none;
    } else if (strcmp(min, "linear") == 0) {
        newFilter.minMode = graphics_FilterMode_linear;
    } else if (strcmp(min, "nearest") == 0) {
        newFilter.minMode = graphics_FilterMode_nearest;
    }

    if (strcmp(mag, "none") == 0) {
        newFilter.magMode = graphics_FilterMode_none;
    } else if (strcmp(mag, "linear") == 0) {
        newFilter.magMode = graphics_FilterMode_linear;
    } else if (strcmp(mag, "nearest") == 0) {
        newFilter.magMode = graphics_FilterMode_nearest;
    }

    graphics_Image_setFilter(img->img, &newFilter);

    return 0;
}

#define DEF_FN(name) { #name, fn_##name }
static const struct fh_named_c_func c_funcs[] = {
    DEF_FN(love_graphics_newImage),
    DEF_FN(love_image_getWidth),
    DEF_FN(love_image_getHeight),
    DEF_FN(love_image_getPath),
    DEF_FN(love_image_getChannels),
    DEF_FN(love_image_refresh),
    DEF_FN(love_image_getDimensions),
    DEF_FN(love_image_getFilter),
    DEF_FN(love_image_setFilter),
    DEF_FN(love_image_getWrap),
    DEF_FN(love_image_setWrap),
};
void fh_image_register(struct fh_program *prog) {
    fh_add_c_funcs(prog, c_funcs, sizeof(c_funcs)/sizeof(c_funcs[0]));
}
