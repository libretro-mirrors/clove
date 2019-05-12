/*
#   clove
#
#   Copyright (C) 2019 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/
#pragma once

#include "../3rdparty/FH/src/fh.h"

#define FH_AUDIO_STATIC_SOURCE_TYPE 10
#define FH_AUDIO_STREAM_SOURCE_TYPE 11

void fh_audio_register(struct fh_program *prog);
