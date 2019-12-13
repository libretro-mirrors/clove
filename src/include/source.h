/*
#   clove
#
#   Copyright (C) 2016-2019 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/
#pragma once

#include <stdbool.h>

#include "../3rdparty/mojoAL/AL/al.h"

typedef enum {
    audio_SourceState_playing,
    audio_SourceState_stopped,
    audio_SourceState_paused
} audio_SourceState;

typedef struct {
    ALuint source;
    audio_SourceState state;
    bool loop;
} audio_SourceCommon;


void audio_SourceCommon_init(audio_SourceCommon *source);
void audio_SourceCommon_play(audio_SourceCommon *source);
void audio_SourceCommon_stop(audio_SourceCommon *source);
void audio_SourceCommon_free(audio_SourceCommon *source);
void audio_SourceCommon_pause(audio_SourceCommon *source);
void audio_SourceCommon_resume(audio_SourceCommon *source);
audio_SourceState audio_SourceCommon_getState(audio_SourceCommon const *source);
bool audio_SourceCommon_isLooping(audio_SourceCommon const* source);
bool audio_SourceCommon_isPlaying(audio_SourceCommon const* source);
bool audio_SourceCommon_isStopped(audio_SourceCommon const* source);
bool audio_SourceCommon_isPaused(audio_SourceCommon const* source);
void audio_SourceCommon_setVolume(audio_SourceCommon const* source, float gain);
float audio_SourceCommon_getVolume(audio_SourceCommon const* source);
float audio_SourceCommon_getPitch(audio_SourceCommon const* source);
void audio_SourceCommon_setPitch(audio_SourceCommon const* source, float gain);
void audio_SourceCommon_setPosition(audio_SourceCommon const* source, float x, float y, float z);
void audio_SourceCommon_setVelocity(audio_SourceCommon const* source, float x, float y, float z);
void audio_SourceCommon_setLooping(audio_SourceCommon const* source, bool value);
