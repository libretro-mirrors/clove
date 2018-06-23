/* Copyright (c) 2018 Murii
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license. See LICENSE for details.
 */

#include "ar_io.h"

#ifdef ARIA_UNIX
    #include <dirent.h>
#elif ARIA_WINDOWS
    #include <WinBase.h>
#endif

ar_Value *f_read(ar_State *S, ar_Value *args) {
    ar_Value *res;
    int r, size;
    const char* file = ar_check_string(S, ar_car(args));
    FILE *fp = fopen(file, "rb");
    if (!fp) ar_error_str(S, "could not open file %s\n", file);
    /* Get size */
    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    /* Load file into string value */
    res = ar_new_stringl(S, NULL, size);
    r = fread(res->u.str.s, 1, size, fp);
    fclose(fp);
    if (r != size) ar_error_str(S, "could not read file %s\n ", file);
    return res;
}


ar_Value *f_write(ar_State *S, ar_Value *args) {
    const char *name, *data;
    int r;
    size_t len;
    FILE *fp;
    name = ar_to_string( S, ar_check(S, ar_nth(args, 0), AR_TSTRING));
    data = ar_to_stringl(S, ar_check(S, ar_nth(args, 1), AR_TSTRING), &len);
    fp = fopen(name, ar_nth(args, 2) ? "ab" : "wb");
    if (!fp) ar_error_str(S, "could not open file");
    r = fwrite(data, len, 1, fp);
    fclose(fp);
    if (r != 1) ar_error_str(S, "could not write file");
    return NULL;
}


/*
 * Reads and loads a custom module packged into a shared library
 */
ar_Value *p_import(ar_State *S, ar_Value *args, ar_Value *env) {
    const char* libname = ar_eval_string(S, ar_car(args), env);
    /*
     * I'm forcing every custom library to have its loading point
     * named 'init_lib'.
     */
    void (*init_lib)();

    #ifdef ARIA_UNIX
    void *handle = dlopen(libname, RTLD_NOW);
    if (!handle) {
        ar_error_str(S, "Failed to load: %s %s\n", libname, dlerror());
    }
    init_lib = dlsym(handle, "init_lib");
    if (!init_lib) {
        ar_error_str(S, "There is no init_lib in library: %s %s\n", libname, dlerror());
    }
    init_lib(S);
    return ar_new_udata(S, handle);
#else /* Windows - Needs more testing */
    HMODULE handle = LoadLibrary(libname);
    if (!handle) {
        ar_error_str(S, "Failed to load: %s \n", libname);
    }
    FARPROC initializer = GetProcAddress(handle, "init_lib");
    if (!initializer) {
        ar_error_str(S, "There is no init_lib in library: %s %s\n", libname);
    }
    init_lib = (void) initializer;
    init_lib(S);
    return ar_new_udata(S, initializer);
    #endif

    return NULL;;
}


ar_Value *p_free_import(ar_State *S, ar_Value *args, ar_Value *env) {
    ar_Value *get_import = ar_eval_udata(S, ar_car(args), env);
    #ifdef ARIA_UNIX
    dlclose(get_import->u.udata.ptr);
    #else /* Windows - Haven't tested it */
    FreeLibrary(get_import->u.udata.ptr);
    #endif
    return NULL;
}


/*
 * Used to open a directory.
 * @Return: a list of names contained in that directory
 */
ar_Value *p_open(ar_State *S, ar_Value *args, ar_Value *env) {

    const char *dirname = ar_eval_string(S, ar_car(args), env);
    ar_Value *ret = NULL;
    DIR *dir;
    struct dirent *ent;

    /* Open directory stream */
    dir = opendir(dirname);

    if (dir != NULL) {
        /* Populate a list with all names inside specified directory */
        while ((ent = readdir (dir)) != NULL) {
            ret = ar_new_pair(S, ar_new_string(S, ent->d_name), ret);
        }
        ret = ar_reverse(ret);
        closedir (dir);
    } else {
        ar_error_str(S, "Cannot open directory %s\n", dirname);
    }
    return ret;
}

ar_Value *p_dirp(ar_State *S, ar_Value *args, ar_Value *env) {
    const char* name = ar_eval_string(S, ar_car(args), env);
    DIR* dir = opendir(name);
    if (dir != NULL)
    {
        ar_Value *ret = readdir(dir) != NULL ? S->t : NULL;
        closedir(dir);
        return ret;
    }
    return NULL;
}

