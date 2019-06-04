/*
#   clove
#
#   Copyright (C) 2019 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/
#include "graphics_particlesystem.h"

#include "../include/particlesystem.h"

#include "../3rdparty/FH/src/value.h"

#include "image.h"

fh_c_obj_gc_callback particle_gc(graphics_ParticleSystem *p) {
    graphics_ParticleSystem_free(p);
    free(p);
    return (fh_c_obj_gc_callback)1;
}

static int fn_love_graphics_newParticleSystem(struct fh_program *prog,
                                      struct fh_value *ret, struct fh_value *args, int n_args) {

    if (!fh_is_c_obj_of_type(&args[0], FH_IMAGE_TYPE))
        return fh_set_error(prog, "Expected image");

    fh_image_t *image = fh_get_c_obj_value(&args[0]);

    double buffer = fh_optnumber(&args[1], n_args, 1, 128);

    graphics_ParticleSystem *p = malloc(sizeof(graphics_ParticleSystem));
    graphics_ParticleSystem_new(p, image->img, (size_t)buffer);

    *ret = fh_new_c_obj(prog, p, particle_gc, FH_GRAPHICS_PARTICLE);

    return 0;
}

#define DEF_FN(name) { #name, fn_##name }
static const struct fh_named_c_func c_funcs[] = {
    DEF_FN(love_graphics_newParticleSystem),
};

void fh_graphics_particlesystem_register(struct fh_program *prog) {
    fh_add_c_funcs(prog, c_funcs, sizeof(c_funcs)/sizeof(c_funcs[0]));
}
