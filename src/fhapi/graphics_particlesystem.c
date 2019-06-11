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

static int fn_love_particleSystem_clone(struct fh_program *prog,
                                        struct fh_value *ret, struct fh_value *args, int n_args) {
    if (!fh_is_c_obj_of_type(&args[0], FH_GRAPHICS_PARTICLE) || !fh_is_c_obj_of_type(&args[1], FH_GRAPHICS_PARTICLE))
        return fh_set_error(prog, "Expected two particle systems");

    graphics_ParticleSystem *cloned = fh_get_c_obj_value(&args[0]);
    graphics_ParticleSystem *to = fh_get_c_obj_value(&args[1]);

    graphics_ParticleSystem_clone(cloned, to);

    *ret = fh_new_c_obj(prog, to, NULL, FH_GRAPHICS_PARTICLE);
    return 0;
}

static int fn_love_particleSystem_setLinearDamping(struct fh_program *prog,
                                                   struct fh_value *ret, struct fh_value *args, int n_args) {
    if (!fh_is_c_obj_of_type(&args[0], FH_GRAPHICS_PARTICLE) || !fh_is_number(&args[1]) || !fh_is_number(&args[2]))
        return fh_set_error(prog, "Expected particle a min and a max");

    graphics_ParticleSystem *p = fh_get_c_obj_value(&args[0]);
    float min = (float)fh_get_number(&args[1]);
    float max = (float)fh_get_number(&args[2]);

    graphics_ParticleSystem_setLinearDamping(p, min, max);

    *ret = fh_new_null();
    return 0;
}

static int fn_love_particleSystem_getLinearDamping(struct fh_program *prog,
                                                   struct fh_value *ret, struct fh_value *args, int n_args) {
    if (!fh_is_c_obj_of_type(&args[0], FH_GRAPHICS_PARTICLE))
        return fh_set_error(prog, "Expected particle");
    float min, max;
    graphics_ParticleSystem *p = fh_get_c_obj_value(&args[0]);
    graphics_ParticleSystem_getLinearDamping(p, &min, &max);

    struct fh_value arr_obj = fh_new_array(prog);
    fh_grow_array(prog, &arr_obj, 2);

    struct fh_array *arr = GET_VAL_ARRAY(&arr_obj);
    arr->items[0] = fh_new_number((double)min);
    arr->items[1] = fh_new_number((double)max);

    *ret = arr_obj;
    return 0;
}

static int fn_love_particleSystem_setBufferSize(struct fh_program *prog,
                                                struct fh_value *ret, struct fh_value *args, int n_args) {
    if (!fh_is_c_obj_of_type(&args[0], FH_GRAPHICS_PARTICLE) || !fh_is_number(&args[1]))
        return fh_set_error(prog, "Expected particle and a size");

    graphics_ParticleSystem *p = fh_get_c_obj_value(&args[0]);
    int size = (int)fh_get_number(&args[1]);

    graphics_ParticleSystem_setBufferSize(p, size);

    *ret = fh_new_null();
    return 0;
}

static int fn_love_particleSystem_getBufferSize(struct fh_program *prog,
                                                struct fh_value *ret, struct fh_value *args, int n_args) {
    if (!fh_is_c_obj_of_type(&args[0], FH_GRAPHICS_PARTICLE))
        return fh_set_error(prog, "Expected particle");

    graphics_ParticleSystem *p = fh_get_c_obj_value(&args[0]);

    *ret = fh_new_number(graphics_ParticleSystem_getBufferSize(p));
    return 0;
}

static int fn_love_particleSystem_setAreaSpread(struct fh_program *prog,
                                                struct fh_value *ret, struct fh_value *args, int n_args) {
    if (!fh_is_c_obj_of_type(&args[0], FH_GRAPHICS_PARTICLE) || !fh_is_string(&args[1]) || !fh_is_number(&args[2]) || !fh_is_number(&args[3]))
        return fh_set_error(prog, "Expected particle a mode:string, dx:number and a dy:number");

    graphics_ParticleSystem *p = fh_get_c_obj_value(&args[0]);
    const char *mode_str = fh_get_string(&args[1]);
    float dx = (float)fh_get_number(&args[2]);
    float dy = (float)fh_get_number(&args[3]);

    graphics_AreaSpreadDistribution mode;

    if (strcmp(mode_str, "uniform") == 0) {
        mode = graphics_AreaSpreadDistribution_uniform;
    } else if (strcmp(mode_str, "normal") == 0) {
        mode = graphics_AreaSpreadDistribution_normal;
    } else if (strcmp(mode_str, "none") == 0) {
        mode = graphics_AreaSpreadDistribution_none;
    } else {
        return fh_set_error(prog, "Invalid area spread distribution mode '%s'", mode_str);
    }

    graphics_ParticleSystem_setAreaSpread(p, mode, dx, dy);

    *ret = fh_new_null();
    return 0;
}

static int fn_love_particleSystem_getAreaSpread(struct fh_program *prog,
                                                struct fh_value *ret, struct fh_value *args, int n_args) {
    if (!fh_is_c_obj_of_type(&args[0], FH_GRAPHICS_PARTICLE))
        return fh_set_error(prog, "Expected particle");

    float dx, dy;
    graphics_AreaSpreadDistribution mode;

    graphics_ParticleSystem *p = fh_get_c_obj_value(&args[0]);
    graphics_ParticleSystem_getAreaSpread(p, &mode, &dx, &dy);

    const char *mode_str;
    switch (mode) {
    case graphics_AreaSpreadDistribution_none:
        mode_str = "none";
        break;
    case graphics_AreaSpreadDistribution_normal:
        mode_str = "normal";
        break;
    case graphics_AreaSpreadDistribution_uniform:
        mode_str = "uniform";
        break;
    }

    struct fh_value arr_obj = fh_new_array(prog);
    fh_grow_array(prog, &arr_obj, 3);

    struct fh_array *arr = GET_VAL_ARRAY(&arr_obj);
    arr->items[0] = fh_new_string(prog, mode_str);
    arr->items[1] = fh_new_number((double)dx);
    arr->items[2] = fh_new_number((double)dy);

    *ret = arr_obj;
    return 0;
}

static int fn_love_particleSystem_setColors(struct fh_program *prog,
                                            struct fh_value *ret, struct fh_value *args, int n_args) {
    if (!fh_is_c_obj_of_type(&args[0], FH_GRAPHICS_PARTICLE) || !fh_is_array(&args[1]))
        return fh_set_error(prog, "Expected particle and an array");

    graphics_ParticleSystem *p = fh_get_c_obj_value(&args[0]);

    struct fh_array *arr = GET_VAL_ARRAY(&args[1]);
    if (arr->len % 4 != 0)
        return fh_set_error(prog, "Array of colors must be power of 4");

    graphics_Color *color = malloc(sizeof(graphics_Color) * (arr->len / 4));

    for (uint32_t i = 0; i < arr->len / 4; i++) {
        color[i].red = (float)fh_get_number(&arr->items[i * 4 + 0]);
        color[i].blue = (float)fh_get_number(&arr->items[i * 4 + 1]);
        color[i].green = (float)fh_get_number(&arr->items[i * 4 + 2]);
        color[i].alpha = (float)fh_get_number(&arr->items[i * 4 + 3]);
    }

    graphics_ParticleSystem_setColors(p, arr->len / 4, color);

    free(color);
    *ret = fh_new_null();
    return 0;
}

static int fn_love_particleSystem_getColors(struct fh_program *prog,
                                            struct fh_value *ret, struct fh_value *args, int n_args) {
    if (!fh_is_c_obj_of_type(&args[0], FH_GRAPHICS_PARTICLE))
        return fh_set_error(prog, "Expected particle");

    graphics_ParticleSystem *p = fh_get_c_obj_value(&args[0]);
    size_t count;

    graphics_Color const *color = graphics_ParticleSystem_getColors(p, &count);

    struct fh_value arr_obj = fh_new_array(prog);
    fh_grow_array(prog, &arr_obj, (uint32_t)count * 4);

    struct fh_array *arr = GET_VAL_ARRAY(&arr_obj);

    for (uint32_t i = 0; i < count; i++) {
        arr->items[i * 4 + 0] = fh_new_number((double)color[i].red);
        arr->items[i * 4 + 1] = fh_new_number((double)color[i].green);
        arr->items[i * 4 + 2] = fh_new_number((double)color[i].blue);
        arr->items[i * 4 + 3] = fh_new_number((double)color[i].alpha);
    }

    *ret = arr_obj;
    return 0;
}

static int fn_love_particleSystem_getCount(struct fh_program *prog,
                                            struct fh_value *ret, struct fh_value *args, int n_args) {
    if (!fh_is_c_obj_of_type(&args[0], FH_GRAPHICS_PARTICLE))
        return fh_set_error(prog, "Expected particle");

    graphics_ParticleSystem *p = fh_get_c_obj_value(&args[0]);
    *ret = fh_new_number((double)graphics_ParticleSystem_getCount(p));

	return 0;
}

static int fn_love_particleSystem_setDirection(struct fh_program *prog,
                                            struct fh_value *ret, struct fh_value *args, int n_args) {
    if (!fh_is_c_obj_of_type(&args[0], FH_GRAPHICS_PARTICLE))
        return fh_set_error(prog, "Expected particle");

    graphics_ParticleSystem *p = fh_get_c_obj_value(&args[0]);
	float dir = (float) fh_get_number(&args[1]);

	graphics_ParticleSystem_setDirection(p, dir);
	*ret = fh_new_null();
	return 0;
}

static int fn_love_particleSystem_setEmissionRate(struct fh_program *prog,
                                            struct fh_value *ret, struct fh_value *args, int n_args) {
    if (!fh_is_c_obj_of_type(&args[0], FH_GRAPHICS_PARTICLE))
        return fh_set_error(prog, "Expected particle");

    graphics_ParticleSystem *p = fh_get_c_obj_value(&args[0]);
	float v = (float) fh_get_number(&args[1]);

	graphics_ParticleSystem_setEmissionRate(p, v);
	*ret = fh_new_null();
	return 0;
}

static int fn_love_particleSystem_setEmitterLifetime(struct fh_program *prog,
                                            struct fh_value *ret, struct fh_value *args, int n_args) {
    if (!fh_is_c_obj_of_type(&args[0], FH_GRAPHICS_PARTICLE))
        return fh_set_error(prog, "Expected particle");

    graphics_ParticleSystem *p = fh_get_c_obj_value(&args[0]);
	float v = (float) fh_get_number(&args[1]);

	graphics_ParticleSystem_setEmitterLifetime(p, v);
	*ret = fh_new_null();
	return 0;
}

static int fn_love_particleSystem_setSizeVariation(struct fh_program *prog,
                                            struct fh_value *ret, struct fh_value *args, int n_args) {
    if (!fh_is_c_obj_of_type(&args[0], FH_GRAPHICS_PARTICLE))
        return fh_set_error(prog, "Expected particle");

    graphics_ParticleSystem *p = fh_get_c_obj_value(&args[0]);
	float v = (float) fh_get_number(&args[1]);

	graphics_ParticleSystem_setSizeVariation(p, v);
	*ret = fh_new_null();
	return 0;
}

static int fn_love_particleSystem_setSpinVariation(struct fh_program *prog,
                                            struct fh_value *ret, struct fh_value *args, int n_args) {
    if (!fh_is_c_obj_of_type(&args[0], FH_GRAPHICS_PARTICLE))
        return fh_set_error(prog, "Expected particle");

    graphics_ParticleSystem *p = fh_get_c_obj_value(&args[0]);
	float v = (float) fh_get_number(&args[1]);

	graphics_ParticleSystem_setSpinVariation(p, v);
	*ret = fh_new_null();
	return 0;
}

static int fn_love_particleSystem_setSpread(struct fh_program *prog,
                                            struct fh_value *ret, struct fh_value *args, int n_args) {
    if (!fh_is_c_obj_of_type(&args[0], FH_GRAPHICS_PARTICLE))
        return fh_set_error(prog, "Expected particle");

    graphics_ParticleSystem *p = fh_get_c_obj_value(&args[0]);
	float v = (float) fh_get_number(&args[1]);

	graphics_ParticleSystem_setSpread(p, v);
	*ret = fh_new_null();
	return 0;
}

static int fn_love_particleSystem_getDirection(struct fh_program *prog,
                                            struct fh_value *ret, struct fh_value *args, int n_args) {
    if (!fh_is_c_obj_of_type(&args[0], FH_GRAPHICS_PARTICLE))
        return fh_set_error(prog, "Expected particle");

    graphics_ParticleSystem *p = fh_get_c_obj_value(&args[0]);
    *ret = fh_new_number((double)graphics_ParticleSystem_getDirection(p));

	return 0;
}

static int fn_love_particleSystem_getEmissionRate(struct fh_program *prog,
                                            struct fh_value *ret, struct fh_value *args, int n_args) {
    if (!fh_is_c_obj_of_type(&args[0], FH_GRAPHICS_PARTICLE))
        return fh_set_error(prog, "Expected particle");

    graphics_ParticleSystem *p = fh_get_c_obj_value(&args[0]);
    *ret = fh_new_number((double)graphics_ParticleSystem_getEmissionRate(p));

	return 0;
}

static int fn_love_particleSystem_getEmitterLifetime(struct fh_program *prog,
                                            struct fh_value *ret, struct fh_value *args, int n_args) {
    if (!fh_is_c_obj_of_type(&args[0], FH_GRAPHICS_PARTICLE))
        return fh_set_error(prog, "Expected particle");

    graphics_ParticleSystem *p = fh_get_c_obj_value(&args[0]);
    *ret = fh_new_number((double)graphics_ParticleSystem_getEmitterLifetime(p));

	return 0;
}

static int fn_love_particleSystem_getSizeVariation(struct fh_program *prog,
                                            struct fh_value *ret, struct fh_value *args, int n_args) {
    if (!fh_is_c_obj_of_type(&args[0], FH_GRAPHICS_PARTICLE))
        return fh_set_error(prog, "Expected particle");

    graphics_ParticleSystem *p = fh_get_c_obj_value(&args[0]);
    *ret = fh_new_number((double)graphics_ParticleSystem_getSizeVariation(p));

	return 0;
}

static int fn_love_particleSystem_getSpinVariation(struct fh_program *prog,
                                            struct fh_value *ret, struct fh_value *args, int n_args) {
    if (!fh_is_c_obj_of_type(&args[0], FH_GRAPHICS_PARTICLE))
        return fh_set_error(prog, "Expected particle");

    graphics_ParticleSystem *p = fh_get_c_obj_value(&args[0]);
    *ret = fh_new_number((double)graphics_ParticleSystem_getSpinVariation(p));

	return 0;
}

static int fn_love_particleSystem_getSpread(struct fh_program *prog,
                                            struct fh_value *ret, struct fh_value *args, int n_args) {
    if (!fh_is_c_obj_of_type(&args[0], FH_GRAPHICS_PARTICLE))
        return fh_set_error(prog, "Expected particle");

    graphics_ParticleSystem *p = fh_get_c_obj_value(&args[0]);
    *ret = fh_new_number((double)graphics_ParticleSystem_getSpread(p));

	return 0;
}

#define DEF_FN(name) { #name, fn_##name }
static const struct fh_named_c_func c_funcs[] = {
    DEF_FN(love_graphics_newParticleSystem),
    DEF_FN(love_particleSystem_clone),
    DEF_FN(love_particleSystem_setLinearDamping),
    DEF_FN(love_particleSystem_getLinearDamping),
    DEF_FN(love_particleSystem_setBufferSize),
    DEF_FN(love_particleSystem_getBufferSize),
    DEF_FN(love_particleSystem_setAreaSpread),
    DEF_FN(love_particleSystem_getAreaSpread),
    DEF_FN(love_particleSystem_setColors),
    DEF_FN(love_particleSystem_getColors),
    DEF_FN(love_particleSystem_getCount),
	DEF_FN(love_particleSystem_setDirection),
	DEF_FN(love_particleSystem_setEmitterLifetime),
	DEF_FN(love_particleSystem_setSizeVariation),
	DEF_FN(love_particleSystem_setSpinVariation),
	DEF_FN(love_particleSystem_setSpread),
	DEF_FN(love_particleSystem_getDirection),
	DEF_FN(love_particleSystem_getEmissionRate),
	DEF_FN(love_particleSystem_getEmitterLifetime),
	DEF_FN(love_particleSystem_getSizeVariation),
	DEF_FN(love_particleSystem_getSpinVariation),
};

void fh_graphics_particlesystem_register(struct fh_program *prog) {
    fh_add_c_funcs(prog, c_funcs, sizeof(c_funcs)/sizeof(c_funcs[0]));
}
