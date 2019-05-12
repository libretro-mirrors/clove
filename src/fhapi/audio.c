/*
#   clove
#
#   Copyright (C) 2019 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/
#include "audio.h"

#include "../3rdparty/FH/src/value.h"

#include "../include/audio.h"
#include "../include/staticsource.h"
#include "../include/streamsource.h"

static void gcStaticSource(audio_StaticSource *source) {
    audio_SourceCommon_free(&source->common);
    free(source);
}


static void gcStreamSource(audio_StreamSource *source) {
    audio_StreamSource_free(source);
    free(source);
}

static int fn_love_audio_newSource(struct fh_program *prog,
                                   struct fh_value *ret, struct fh_value *args, int n_args) {
    if (!fh_is_string(&args[0]))
        return fh_set_error(prog, "Expected filename:string");

    const char *filename = fh_get_string(&args[0]);
    const char *type = fh_optstring(args, n_args, 1, "static");
    int err = 1;
    audio_StaticSource *staticSource = NULL;
    audio_StreamSource *streamSource = NULL;
    if (strcmp(type, "static") == 0) {
        fh_c_obj_gc_callback *callback = gcStaticSource;

        staticSource = malloc(sizeof(audio_StaticSource));
        err = audio_loadStatic(staticSource, filename);
        *ret = fh_new_c_obj(prog, staticSource, callback, FH_AUDIO_STATIC_SOURCE_TYPE);
    } else if (strcmp(type, "stream") == 0) {
        fh_c_obj_gc_callback *callback = gcStreamSource;

        streamSource = malloc(sizeof(audio_StreamSource));
        err = audio_loadStream(streamSource, filename);
        *ret = fh_new_c_obj(prog, streamSource, callback, FH_AUDIO_STREAM_SOURCE_TYPE);
    }
    if (err == -1) {
        if (staticSource)
            free(staticSource);
        if (streamSource)
            free(streamSource);
        return fh_set_error(prog, "Could not load sound file %s, reason: unknown file type", filename);
    } else if (err == 0) {
        if (staticSource)
            free(staticSource);
        if (streamSource)
            free(streamSource);
        return fh_set_error(prog, "Could not load sound file %s, file does not exist", filename);
    }
    return 0;
}

static int fn_love_audio_play(struct fh_program *prog,
                                   struct fh_value *ret, struct fh_value *args, int n_args) {
    if (fh_is_c_obj_of_type(&args[0], FH_AUDIO_STATIC_SOURCE_TYPE)) {
        audio_StaticSource *source = fh_get_c_obj_value(&args[0]);
        audio_StaticSource_play(source);
    } else if (fh_is_c_obj_of_type(&args[0], FH_AUDIO_STREAM_SOURCE_TYPE)) {
        audio_StreamSource *source = fh_get_c_obj_value(&args[0]);
        audio_StreamSource_play(source);
    }
    else
        return fh_set_error(prog, "Expected stream or static audio source");

    *ret = fh_new_null();
    return 0;
}

#define DEF_FN(name) { #name, fn_##name }
static const struct fh_named_c_func c_funcs[] = {
    DEF_FN(love_audio_newSource),
    DEF_FN(love_audio_play),

};

void fh_audio_register(struct fh_program *prog) {
    fh_add_c_funcs(prog, c_funcs, sizeof(c_funcs)/sizeof(c_funcs[0]));
}
