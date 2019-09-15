/*
#   clove
#
#   Copyright (C) 2019 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/

#include "ui.h"

#include "../3rdparty/FH/src/value.h"

#include "../3rdparty/microui/src/microui.h"
#include "../include/ui.h"

static fh_c_obj_gc_callback ui_container_gc(mu_Container *cnt) {
    free(cnt);
    return (fh_c_obj_gc_callback)1;
}

static int fn_love_ui_newWindow(struct fh_program *prog,
                                   struct fh_value *ret, struct fh_value *args, int n_args) {
    if (n_args != 4) {
        return fh_set_error(prog, "Invalid number of arguments, expected 4 got: %d", n_args);
    }
    mu_Container *cnt = malloc(sizeof(mu_Container));

    for (int i = 0; i < 4; i++) {
        if (!fh_is_number(&args[i])) {
            return fh_set_error(prog, "Argument %d must be a number", i);
        }
    }

    int x = (int)fh_get_number(&args[0]);
    int y = (int)fh_get_number(&args[1]);
    int w = (int)fh_get_number(&args[2]);
    int h = (int)fh_get_number(&args[3]);

    ui_init_window(cnt, x, y, w, h, 0);

    fh_c_obj_gc_callback *callback = ui_container_gc;
    *ret = fh_new_c_obj(prog, cnt, callback, FH_UI_TYPE);

    return 0;
}

static int fn_love_ui_begin_window(struct fh_program *prog,
                                   struct fh_value *ret, struct fh_value *args, int n_args) {
    if (n_args != 2) {
        return fh_set_error(prog, "Invalid number of arguments, expected 2 got: %d", n_args);
    }

    if (!fh_is_c_obj_of_type(&args[0], FH_UI_TYPE)) {
        return fh_set_error(prog, "Expected argument 0 to be an ui window");
    }

    mu_Container *cnt = fh_get_c_obj_value(&args[0]);
    const char *title = fh_get_string(&args[1]);

    *ret = fh_new_bool(ui_begin_window(title, cnt));
    return 0;
}

static int fn_love_ui_end_window(struct fh_program *prog,
                                   struct fh_value *ret, struct fh_value *args, int n_args) {
    UNUSED(prog);
    UNUSED(args);
    UNUSED(n_args);
    ui_end_window();
    *ret = fh_new_null();
    return 0;
}

static int fn_love_ui_header(struct fh_program *prog,
                                   struct fh_value *ret, struct fh_value *args, int n_args) {
    if (!fh_is_string(&args[0])) {
        return fh_set_error(prog, "Expected string for argument 0");
    }
    if (!fh_is_bool(&args[1])) {
        return fh_set_error(prog, "Expected boolean for argument 1");

    }
    const char *title = fh_get_string(&args[0]);
    int open = fh_get_bool(&args[1]);
    int opt = (int) fh_optnumber(args, n_args, 2, MU_OPT_ALIGNLEFT);
    *ret = fh_new_bool(ui_header(&open, title, opt));
    return 0;
}

static int fn_love_ui_begin_tree(struct fh_program *prog,
                                   struct fh_value *ret, struct fh_value *args, int n_args) {
    if (!fh_is_string(&args[0])) {
        return fh_set_error(prog, "Expected string for argument 0");
    }
    if (!fh_is_bool(&args[1])) {
        return fh_set_error(prog, "Expected boolean for argument 1");

    }
    const char *title = fh_get_string(&args[0]);
    int open = fh_get_bool(&args[1]);
    *ret = fh_new_bool(ui_begin_tree(&open, title));
    return 0;
}

static int fn_love_ui_end_tree(struct fh_program *prog,
                                   struct fh_value *ret, struct fh_value *args, int n_args) {
    UNUSED(prog);
    UNUSED(args);
    UNUSED(n_args);
    ui_end_tree();
    *ret = fh_new_null();
    return 0;
}

static int fn_love_ui_slider(struct fh_program *prog,
                                   struct fh_value *ret, struct fh_value *args, int n_args) {
    if (n_args < 3) {
        return fh_set_error(prog, "Expected at least 3 arguments");
    }
    for (int i = 0; i < 3; i++) {
        if (!fh_is_number(&args[i])) {
            return fh_set_error(prog, "Argument %d was expected to be of type number", i);
        }
    }
    double value = fh_get_number(&args[0]);
    double low = fh_get_number(&args[1]);
    double high = fh_get_number(&args[2]);
    int opt = (int) fh_optnumber(args, n_args, 3, MU_OPT_ALIGNLEFT);
    *ret = fh_new_bool(ui_slider(value, low, high, opt));
    return 0;
}

static int fn_love_ui_label(struct fh_program *prog,
                                   struct fh_value *ret, struct fh_value *args, int n_args) {
    const char *label = fh_get_string(&args[0]);
    int opt = (int) fh_optnumber(args, n_args, 1, MU_OPT_ALIGNLEFT);
    ui_label(label, opt);
    *ret = fh_new_bool(true);
    return 0;
}

static int fn_love_ui_button(struct fh_program *prog,
                                   struct fh_value *ret, struct fh_value *args, int n_args) {
    const char *label = fh_get_string(&args[0]);
    int opt = (int) fh_optnumber(&args[1], n_args, 1, MU_OPT_ALIGNLEFT);
    *ret = fh_new_bool(ui_button(label, opt));
    return 0;
}

static int fn_love_ui_textbox(struct fh_program *prog,
                                   struct fh_value *ret, struct fh_value *args, int n_args) {
    const char *label = fh_get_string(&args[0]);
    int opt = (int) fh_optnumber(&args[1], n_args, 1, MU_OPT_ALIGNLEFT);
    *ret = fh_new_bool(ui_textbox(label, opt));
    return 0;
}

static int fn_love_ui_begin(struct fh_program *prog,
                                   struct fh_value *ret, struct fh_value *args, int n_args) {
    UNUSED(prog);
    UNUSED(args);
    UNUSED(n_args);
    ui_begin();
    *ret = fh_new_null();
    return 0;
}

static int fn_love_ui_end(struct fh_program *prog,
                                   struct fh_value *ret, struct fh_value *args, int n_args) {
    UNUSED(prog);
    UNUSED(args);
    UNUSED(n_args);
    ui_end();
    *ret = fh_new_null();
    return 0;
}

#define DEF_FN(name) { #name, fn_##name }
static const struct fh_named_c_func c_funcs[] = {
    DEF_FN(love_ui_newWindow),
    DEF_FN(love_ui_begin_tree),
    DEF_FN(love_ui_end_tree),
    DEF_FN(love_ui_begin_window),
    DEF_FN(love_ui_end_window),
    DEF_FN(love_ui_header),
    DEF_FN(love_ui_button),
    DEF_FN(love_ui_textbox),
    DEF_FN(love_ui_label),
    DEF_FN(love_ui_slider),
    DEF_FN(love_ui_begin),
    DEF_FN(love_ui_end),
};


void fh_ui_register(struct fh_program *prog) {
    fh_add_c_funcs(prog, c_funcs, sizeof(c_funcs)/sizeof(c_funcs[0]));
}
