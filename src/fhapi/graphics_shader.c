/*
#   clove
#
#   Copyright (C) 2019 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/
#include "graphics_shader.h"

#include "../3rdparty/FH/src/value.h"
#include "../3rdparty/slre/slre.h"

#include <string.h>
#include <stdlib.h>

#include "../include/filesystem.h"
#include "../include/minmax.h"

#include "image.h"

static fh_c_obj_gc_callback shader_gc(fh_graphics_Shader *shader) {
    graphics_Shader_free(&shader->shader);
    free(shader->referencedTextures);
    free(shader);
    return (fh_c_obj_gc_callback) 1;
}

static struct {
    struct slre fragmentSingleShaderDetectRegex;
    struct slre vertexShaderDetectRegex;
    void * sendValueBuffer;
    int sendValueBufferSize;
    fh_graphics_Shader *currentShader;
} moduleData;

static char const * fragmentSingleShaderDetectRegexSrc = "vec4\\s*effect\\s*\\(";
static char const * vertexShaderDetectRegexSrc = "vec4\\s*position\\s*\\(";

bool static isVertexShader(char const* str) {
    return slre_match(&moduleData.vertexShaderDetectRegex, str, strlen(str), NULL);
}

bool static isSingleFragmentShader(char const* str) {
    return slre_match(&moduleData.fragmentSingleShaderDetectRegex, str, strlen(str), NULL);
}

static int fn_love_graphics_newShader(struct fh_program *prog,
                                      struct fh_value *ret, struct fh_value *args, int n_args) {
    if (!fh_is_string(&args[0]))
        return fh_set_error(prog, "Expected vertex source");

    const char *vertexSrc = fh_get_string(&args[0]);
    const char *fragmentSrc = NULL;

    char *loadedFile1 = NULL;
    char *loadedFile2 = NULL;

    if (fh_is_string(&args[1])) {
        fragmentSrc = fh_get_string(&args[1]);

        if (!isVertexShader(vertexSrc)) {
            // TODO
            filesystem_read(vertexSrc, &loadedFile1);
            if (!loadedFile1 || !isVertexShader(loadedFile1)) {
                free(loadedFile1);
                return fh_set_error(prog, "input 1 is not a valid vertex shader");
            }
            vertexSrc = loadedFile1;
        }

        if(!isSingleFragmentShader(fragmentSrc)) {
            // TODO
            filesystem_read(fragmentSrc, &loadedFile2);
            if(!loadedFile2 || !isSingleFragmentShader(loadedFile2)) {
                free(loadedFile1);
                free(loadedFile2);
                return fh_set_error(prog, "input 2 is not a valid fragment shader");
            }
            fragmentSrc = loadedFile2;
        }
    } else {
        if(isVertexShader(vertexSrc)) {
            // nothing required
        } else if(isSingleFragmentShader(vertexSrc)) {
            fragmentSrc = vertexSrc;
            vertexSrc = NULL;
        } else {
            // TODO
            int loadedFile1Size = filesystem_read(vertexSrc, &loadedFile1);
            (void) loadedFile1Size;
            if(!loadedFile1) {
                return fh_set_error(prog, "could not open file");
            }

            if(isSingleFragmentShader(loadedFile1)) {
                fragmentSrc = loadedFile1;
                vertexSrc = NULL;
            } else if(isVertexShader(loadedFile1)) {
                vertexSrc = loadedFile1;
                fragmentSrc = NULL;
            } else {
                free(loadedFile1);
                return fh_set_error(prog, "input is not a valid shader");
            }
        }
    }

    fh_graphics_Shader *shader = malloc(sizeof(fh_graphics_Shader));
    graphics_ShaderCompileStatus status = graphics_Shader_new(&shader->shader, vertexSrc, fragmentSrc);
    switch (status) {
    case graphics_ShaderCompileStatus_okay:
        break;
    case graphics_ShaderCompileStatus_linkError:
    {
        free(loadedFile1);
        free(loadedFile2);
        return fh_set_error(prog, "Failed to link shader!");
    }
    case graphics_ShaderCompileStatus_vertexError:
    {
        free(loadedFile1);
        free(loadedFile2);
        return fh_set_error(prog, "Failed to compile vertex shader!");
    }
    case graphics_ShaderCompileStatus_fragmentError:
    {
        free(loadedFile1);
        free(loadedFile2);
        return fh_set_error(prog, "Failed to compile fragment shader!");
    }
    }

    free(loadedFile1);
    free(loadedFile2);

    int const textureUnits = shader->shader.textureUnitCount;
    shader->referencedTextures = malloc(textureUnits * sizeof(int));

    *ret = fh_new_c_obj(prog, shader, shader_gc, FH_GRAPHICS_SHADER);

    return 0;
}

static int fn_love_graphics_getShader(struct fh_program *prog,
                                      struct fh_value *ret, struct fh_value *args, int n_args) {
    *ret = fh_new_c_obj(prog, moduleData.currentShader, NULL, FH_GRAPHICS_SHADER);
    return 0;
}

static int fn_love_graphics_setShader(struct fh_program *prog,
                                      struct fh_value *ret, struct fh_value *args, int n_args) {
    if (fh_is_null(&args[0])) {
        graphics_setDefaultShader();
        moduleData.currentShader = NULL;
    } else if (fh_is_c_obj_of_type(&args[0], FH_GRAPHICS_SHADER)) {
        moduleData.currentShader = fh_get_c_obj_value(&args[0]);
        graphics_setShader(&moduleData.currentShader->shader);
    } else {
        return fh_set_error(prog, "expected null or shader");
    }
    *ret = fh_new_null();
    return 0;
}

#define DEF_FN(name) { #name, fn_##name }
static const struct fh_named_c_func c_funcs[] = {
    DEF_FN(love_graphics_newShader),
    DEF_FN(love_graphics_setShader),
    DEF_FN(love_graphics_getShader),
};

void fh_graphics_shader_register(struct fh_program *prog) {
    moduleData.currentShader = NULL;

    slre_compile(&moduleData.fragmentSingleShaderDetectRegex, fragmentSingleShaderDetectRegexSrc);
    slre_compile(&moduleData.vertexShaderDetectRegex, vertexShaderDetectRegexSrc);

    fh_add_c_funcs(prog, c_funcs, sizeof(c_funcs)/sizeof(c_funcs[0]));
}
