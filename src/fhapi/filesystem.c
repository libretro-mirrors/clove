/*
#   clove
#
#   Copyright (C) 2019 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/
#include "filesystem.h"

#include "../3rdparty/FH/src/value.h"

#include "../include/filesystem.h"

static int fn_love_filesystem_read(struct fh_program *prog,
                                   struct fh_value *ret, struct fh_value *args, int n_args) {
    if (!fh_is_string(&args[0]))
        return fh_set_error(prog, "Illegal parameter, expected filename:string");

    const char *filename = fh_get_string(&args[0]);
    char *data = NULL;
    int len = filesystem_read(filename, &data);
    if (len < -1) {
        return fh_set_error(prog, "Could not open file %s\n", filename);
    }

    *ret = fh_new_string(prog, data);
    free(data);
    return 0;
}

static int fn_love_filesystem_write(struct fh_program *prog,
                                    struct fh_value *ret, struct fh_value *args, int n_args) {
    if (!fh_is_string(&args[0]) || !fh_is_string(&args[1]))
        return fh_set_error(prog, "Illegal parameter, expected filename:string and data:string");

    const char *filename = fh_get_string(&args[0]);
    const char *data = fh_get_string(&args[1]);
    int wrote_data = filesystem_write(filename, data);
    if (wrote_data < 0) {
        //Note: error will be issued in filesystem_write, no need to respecify it here.
        return -1;
    }

    *ret = fh_new_number(wrote_data);
    return 0;
}

static int fn_love_filesystem_append(struct fh_program *prog,
                                     struct fh_value *ret, struct fh_value *args, int n_args) {
    if (!fh_is_string(&args[0]) || !fh_is_string(&args[1]))
        return fh_set_error(prog, "Illegal parameter, expected filename:string and data:string");

    const char *filename = fh_get_string(&args[0]);
    const char *data = fh_get_string(&args[1]);
    int appended_data = filesystem_append(filename, data);
    if (appended_data < 0) {
        //Note: error will be issued in filesystem_write, no need to respecify it here.
        return -1;
    }

    *ret = fh_new_number(appended_data);
    return 0;
}

static int fn_love_filesystem_getSaveDirectory(struct fh_program *prog,
                                               struct fh_value *ret, struct fh_value *args, int n_args) {
    const char *company = fh_optstring(args, n_args, 0, "CLove");
    const char *projName = fh_optstring(args, n_args, 1, "myGame");

    const char *path = filesystem_getSaveDirectory(company, projName);
    *ret = fh_new_string(prog, path);
    SDL_free((char*)path);
    return 0;
}

static int fn_love_filesystem_exists(struct fh_program *prog,
                                     struct fh_value *ret, struct fh_value *args, int n_args) {
    if (!fh_is_string(&args[0]))
        return fh_set_error(prog, "Illegal parameter, expected filename:string");
    const char *filename = fh_get_string(&args[0]);

    *ret = fh_new_bool(filesystem_exists(filename));
    return 0;
}

static int fn_love_filesystem_getSource(struct fh_program *prog,
                                        struct fh_value *ret, struct fh_value *args, int n_args) {
    const char *source = filesystem_getSource();
    *ret = fh_new_string(prog, source);
    free((char*)source);
    return 0;
}

static int fn_love_filesystem_remove(struct fh_program *prog,
                                     struct fh_value *ret, struct fh_value *args, int n_args) {
    if (!fh_is_string(&args[0]))
        return fh_set_error(prog, "Illegal parameter, expected filename:string");
    const char *filename = fh_get_string(&args[0]);

    *ret = fh_new_bool(filesystem_remove(filename));
    return 0;
}

static int fn_love_filesystem_rename(struct fh_program *prog,
                                     struct fh_value *ret, struct fh_value *args, int n_args) {
    if (!fh_is_string(&args[0]) || !fh_is_string(&args[1]))
        return fh_set_error(prog, "Illegal parameter, expected old_filename:string and new_filename:string");
    const char *old_filename = fh_get_string(&args[0]);
    const char *new_filename = fh_get_string(&args[1]);

    *ret = fh_new_bool(filesystem_rename(old_filename, new_filename));
    return 0;
}

static int fn_love_filesystem_status(struct fh_program *prog,
                                     struct fh_value *ret, struct fh_value *args, int n_args) {
    if (!fh_is_string(&args[0]))
        return fh_set_error(prog, "Illegal parameter, expected filename:string");
    const char *filename = fh_get_string(&args[0]);
    const char *smode = fh_optstring(args, n_args, 1, "e");

    int mode = 0;

    if (strcmp(smode, "e") == 0) {
        mode = 0;
    } else if (strcmp(smode, "x") == 0) {
        mode = 1;
    } else if (strcmp(smode, "w") == 0) {
        mode = 2;
    } else if (strcmp(smode, "r") == 0) {
        mode = 4;
    } else if (strcmp(smode, "wr") == 0 || strcmp(smode, "rw") == 0) {
        mode = 6;
    }

    *ret = fh_new_bool(filesystem_status(filename, mode));
    return 0;
}

#define DEF_FN(name) { #name, fn_##name }
static const struct fh_named_c_func c_funcs[] = {
    DEF_FN(love_filesystem_read),
    DEF_FN(love_filesystem_write),
    DEF_FN(love_filesystem_append),
    DEF_FN(love_filesystem_getSaveDirectory),
    DEF_FN(love_filesystem_exists),
    DEF_FN(love_filesystem_getSource),
    DEF_FN(love_filesystem_remove),
    DEF_FN(love_filesystem_rename),
    DEF_FN(love_filesystem_status),
};

void fh_filesystem_register(struct fh_program *prog) {
    fh_add_c_funcs(prog, c_funcs, sizeof(c_funcs)/sizeof(c_funcs[0]));
}
