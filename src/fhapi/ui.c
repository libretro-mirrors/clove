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

static int fn_love_ui_newContainer(struct fh_program *prog,
                                   struct fh_value *ret, struct fh_value *args, int n_args) {

        mu_Container *cnt = malloc(sizeof(mu_Container));
        fh_c_obj_gc_callback *callback = ui_container_gc;
        *ret = fh_new_c_obj(prog, cnt, callback, FH_UI_TYPE);
        return 0;
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

static int fn_love_ui_layout_row(struct fh_program *prog,
                                   struct fh_value *ret, struct fh_value *args, int n_args) {
    if (n_args != 3) {
        return fh_set_error(prog, "Invalid number of arguments, expected 3 got: %d", n_args);
    }

    if (!fh_is_number(&args[0])) {
        return fh_set_error(prog, "Expected number as argument 0");
    }
    if (!fh_is_number(&args[1])) {
        return fh_set_error(prog, "Expected number as argument 1");
    }

    int no_items = (int)fh_get_number(&args[0]);
    struct fh_value *arr = &args[1];
    int len = fh_get_array_len(arr);
    int *size = malloc(sizeof(int)*len);

    struct fh_array *a = GET_VAL_ARRAY(arr);
    for (int i = 0; i < len; i++) {
        if (a->items[i].type != FH_VAL_FLOAT) {
            free (size);
            return fh_set_error(prog, "Expected index %d in array to be of type number, got %s", i, fh_type_to_str(prog, a->items[i].type));
        }
        size[i] = (int)a->items[i].data.num;
    }
    int height = (int)fh_get_number(&args[2]);

    ui_layout_row(no_items, size, height);
    free (size);
    *ret = fh_new_null();
    return 0;
}

static int fn_love_ui_header(struct fh_program *prog,
                                   struct fh_value *ret, struct fh_value *args, int n_args) {
    if (!fh_is_string(&args[0])) {
        return fh_set_error(prog, "Expected string for argument 0");
    }
    //if (!fh_is_bool(&args[1])) {
      //  return fh_set_error(prog, "Expected boolean for argument 1");
    //}
    const char *title = fh_get_string(&args[0]);
    int open = fh_get_bool(&args[1]);
    int id = fh_get_number(&args[2]);
    int opt = (int) fh_optnumber(args, n_args, 3, MU_OPT_ALIGNLEFT);
    *ret = fh_new_bool(ui_header(open, title, id, opt));
    return 0;
}

static int fn_love_ui_begin_tree(struct fh_program *prog,
                                   struct fh_value *ret, struct fh_value *args, int n_args) {
    if (!fh_is_string(&args[0])) {
        return fh_set_error(prog, "Expected string for argument 0");
    } else if (!fh_is_bool(&args[1])) {
        return fh_set_error(prog, "Expected boolean for argument 1");
    } else if (!fh_is_number(&args[2])) {
        return fh_set_error(prog, "Expected id (number) for argument 2");
    }
    const char *title = fh_get_string(&args[0]);
    int open = fh_get_bool(&args[1]);
    int id = fh_get_number(&args[2]);
    *ret = fh_new_bool(ui_begin_tree(open, title, id));
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

static int fn_love_ui_begin_panel(struct fh_program *prog,
                struct fh_value *ret, struct fh_value *args, int n_args) {
        if (n_args != 1) {
                return fh_set_error(prog, "Expected at least 1 arguments");
        }
        if (!fh_is_c_obj_of_type(&args[0], FH_UI_TYPE)) {
                return fh_set_error(prog, "Expected argument 0 to be an ui window");
        }

        mu_Container *cnt = fh_get_c_obj_value(&args[0]);
        int opt = (int) fh_optnumber(args, n_args, 1, MU_OPT_ALIGNLEFT);
        ui_begin_panel(cnt, opt);

        *ret = fh_new_bool(true);
        return 0;
}

static int fn_love_ui_end_panel(struct fh_program *prog,
                struct fh_value *ret, struct fh_value *args, int n_args) {
        UNUSED(prog);
        UNUSED(ret);
        UNUSED(args);
        ui_end_panel();

        *ret = fh_new_bool(true);
        return 0;
}

static int fn_love_ui_draw_text(struct fh_program *prog,
                struct fh_value *ret, struct fh_value *args, int n_args) {
        if (!fh_is_string(&args[0])) {
                return fh_set_error(prog, "Expected string for argument 0");
        }
        const char *text = fh_get_string(&args[0]);
        ui_draw_text(text);
        *ret = fh_new_bool(true);
        return 0;
}

static int fn_love_ui_last_id(struct fh_program *prog,
                struct fh_value *ret, struct fh_value *args, int n_args) {
        *ret = fh_new_number(ui_get_context()->last_id);
        return 0;
}

static int fn_love_ui_set_focus(struct fh_program *prog,
                struct fh_value *ret, struct fh_value *args, int n_args) {
        if (!fh_is_number(&args[0])) {
                return fh_set_error(prog, "expected id as number");
        }
        int id = fh_get_number(&args[0]);
        mu_set_focus(ui_get_context(), id);
        *ret = fh_new_bool(true);
        return 0;
}

static int fn_love_ui_slider(struct fh_program *prog,
                                   struct fh_value *ret, struct fh_value *args, int n_args)
{
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

static int fn_love_ui_align(struct fh_program *prog,
                                   struct fh_value *ret, struct fh_value *args, int n_args) {
        if (!fh_is_string(&args[0])) {
                return fh_set_error(prog, "Expected string!");
        }
        const char *align = fh_get_string(&args[0]);
        int num = 0;
        if (strcmp(align, "left") == 0) {
                num = MU_OPT_ALIGNLEFT;
        } else if (strcmp(align, "center") == 0) {
                num = MU_OPT_ALIGNCENTER;
        } else if (strcmp(align, "right") == 0) {
                num = MU_OPT_ALIGNRIGHT;
        } else if (strcmp(align, "bottom") == 0) {
                num = MU_OPT_ALIGNBOTTOM;
        } else {
                return fh_set_error(prog, "invalid alignment %s\n", align);
        }
        *ret = fh_new_number(num);
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
    *ret = fh_new_bool(ui_textbox((char*)label, opt));
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
    DEF_FN(love_ui_layout_row),
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
    DEF_FN(love_ui_align),
    DEF_FN(love_ui_begin_panel),
    DEF_FN(love_ui_end_panel),
    DEF_FN(love_ui_draw_text),
    DEF_FN(love_ui_set_focus),
    DEF_FN(love_ui_last_id),
};


void fh_ui_register(struct fh_program *prog) {
    fh_add_c_funcs(prog, c_funcs, sizeof(c_funcs)/sizeof(c_funcs[0]));
}
