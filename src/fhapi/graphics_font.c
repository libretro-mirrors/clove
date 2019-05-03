/*
#   clove
#
#   Copyright (C) 2019 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/

#include "graphics_font.h"

#include "../3rdparty/FH/src/value.h"

#include "../include/font.h"
#include "../include/matrixstack.h"
#include "../include/graphics.h"

static struct {
    graphics_Font defaultFont;
    graphics_Font* currentFont;
    int currentFontSize;
} moduleData;

static void gcFont(graphics_Font *font) {
    graphics_Font_free(font);
    free(font);
}

static int fn_love_graphics_newFont(struct fh_program *prog,
                                    struct fh_value *ret, struct fh_value *args, int n_args) {
    const char *filename = NULL;
    moduleData.currentFontSize = 12;

    if (!fh_is_null(&args[0])) {
        enum fh_value_type type = args[0].type;
        if (type == FH_VAL_STRING) {
            filename = fh_get_string(&args[0]);
            if (fh_is_number(&args[1])) {
                moduleData.currentFontSize = (int)fh_get_number(&args[1]);
            }
        } else if (type == FH_VAL_FLOAT){
            moduleData.currentFontSize = (int)fh_get_number(&args[0]);
        } else {
            return fh_set_error(prog, "Illegal parameter, expected null, string or number for the first argument");
        }
    }

    graphics_Font *font = malloc(sizeof(graphics_Font));
    graphics_Font_new(font, filename, moduleData.currentFontSize);

    fh_c_obj_gc_callback freeCallback = gcFont;
    *ret = fh_new_c_obj(prog, font, freeCallback, FH_FONT_TYPE);
    return 0;
}

static void graphics_loadDefaultFont() {
    moduleData.currentFontSize = 12;
    graphics_Font_new(&moduleData.defaultFont, NULL, 12);
    moduleData.currentFont = &moduleData.defaultFont;
}

static int fn_love_graphics_setFont(struct fh_program *prog,
                                    struct fh_value *ret, struct fh_value *args, int n_args) {

    if (!fh_is_c_obj_of_type(&args[0], FH_FONT_TYPE)) {
        return fh_set_error(prog, "Expected font");
    }

    moduleData.currentFont = fh_get_c_obj_value(&args[0]);

    *ret = fh_new_null();
    return 0;
}

static int fn_love_graphics_getFont(struct fh_program *prog,
                                    struct fh_value *ret, struct fh_value *args, int n_args) {
    *ret = fh_new_c_obj(prog, moduleData.currentFont, NULL, FH_FONT_TYPE);
    return 0;
}

static int fn_love_graphics_print(struct fh_program *prog,
                                  struct fh_value *ret, struct fh_value *args, int n_args) {
    if (!fh_is_string(&args[0]))
        return fh_set_error(prog, "Expected string");

    if (!moduleData.currentFont) {
        graphics_loadDefaultFont();
    }

    const char *text = fh_get_string(&args[0]);

    int x = fh_optnumber(args, n_args, 1, 0);
    int y = fh_optnumber(args, n_args, 2, 0);
    int r = fh_optnumber(args, n_args, 3, 0);
    int sx = fh_optnumber(args, n_args, 4, 1.0);
    int sy = fh_optnumber(args, n_args, 5, 1.0);
    int ox = fh_optnumber(args, n_args, 6, 0);
    int oy = fh_optnumber(args, n_args, 7, 0);
    int kx = fh_optnumber(args, n_args, 8, 0);
    int ky = fh_optnumber(args, n_args, 9, 0);

    graphics_Font_render(moduleData.currentFont, text, x, y, r, sx, sy, ox, oy, kx, ky);

    *ret = fh_new_null();
    return 0;
}

static int fn_love_font_getHeight(struct fh_program *prog,
                                    struct fh_value *ret, struct fh_value *args, int n_args) {
    if (!fh_is_c_obj_of_type(&args[0], FH_FONT_TYPE)) {
        return fh_set_error(prog, "Expected font");
    }
    graphics_Font *font = fh_get_c_obj_value(&args[0]);

    *ret = fh_new_number((int)graphics_Font_getHeight(font));
    return 0;
}

static int fn_love_font_getWidth(struct fh_program *prog,
                                    struct fh_value *ret, struct fh_value *args, int n_args) {
    if (n_args != 2) {
        return fh_set_error(prog, "Illegal number of arguments, expected two, font and string");
    }
    if (!fh_is_c_obj_of_type(&args[0], FH_FONT_TYPE) || !fh_is_string(&args[1])) {
        return fh_set_error(prog, "Expected font and string");
    }
    graphics_Font *font = fh_get_c_obj_value(&args[0]);
    char const *line = fh_get_string(&args[1]);

    *ret = fh_new_number((int)graphics_Font_getWidth(font, line));
    return 0;
}

static int fn_love_font_getDescent(struct fh_program *prog,
                                    struct fh_value *ret, struct fh_value *args, int n_args) {
    if (!fh_is_c_obj_of_type(&args[0], FH_FONT_TYPE)) {
        return fh_set_error(prog, "Expected font");
    }
    graphics_Font *font = fh_get_c_obj_value(&args[0]);

    *ret = fh_new_number((int)graphics_Font_getDescent(font));
    return 0;
}

static int fn_love_font_getAscent(struct fh_program *prog,
                                    struct fh_value *ret, struct fh_value *args, int n_args) {
    if (!fh_is_c_obj_of_type(&args[0], FH_FONT_TYPE)) {
        return fh_set_error(prog, "Expected font");
    }
    graphics_Font *font = fh_get_c_obj_value(&args[0]);

    *ret = fh_new_number((int)graphics_Font_getAscent(font));
    return 0;
}

static int fn_love_font_getBaseline(struct fh_program *prog,
                                    struct fh_value *ret, struct fh_value *args, int n_args) {
    if (!fh_is_c_obj_of_type(&args[0], FH_FONT_TYPE)) {
        return fh_set_error(prog, "Expected font");
    }
    graphics_Font *font = fh_get_c_obj_value(&args[0]);

    *ret = fh_new_number((int)graphics_Font_getBaseline(font));
    return 0;
}


static int fn_love_font_getWrap(struct fh_program *prog,
                                    struct fh_value *ret, struct fh_value *args, int n_args) {
    if (!fh_is_c_obj_of_type(&args[0], FH_FONT_TYPE)) {
        return fh_set_error(prog, "Expected font");
    }
    graphics_Font *font = fh_get_c_obj_value(&args[0]);

    const char *line = fh_get_string(&args[1]);
    int width = fh_get_number(&args[2]);

    //TODO http://muriidev.duckdns.org:3001/murii/CLove/issues/2

    //*ret = fh_new_number((int)graphics_Font_getWrap(font, line, width, NULL));
    return 0;
}

#define DEF_FN(name) { #name, fn_##name }
static const struct fh_named_c_func c_funcs[] = {
    DEF_FN(love_graphics_newFont),
    DEF_FN(love_graphics_setFont),
    DEF_FN(love_graphics_getFont),
    DEF_FN(love_font_getHeight),
    DEF_FN(love_font_getWidth),
    DEF_FN(love_font_getBaseline),
    DEF_FN(love_font_getAscent),
    DEF_FN(love_font_getDescent),
    DEF_FN(love_graphics_print),
};

void fh_graphics_font_register(struct fh_program *prog) {
    moduleData.currentFont = NULL;

    fh_add_c_funcs(prog, c_funcs, sizeof(c_funcs)/sizeof(c_funcs[0]));
}
