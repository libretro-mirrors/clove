/*
#   clove
#
#   Copyright (C) 2016-2017 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/
#pragma once

#include "../3rdparty/openal/include/AL/al.h"

typedef struct {
  ALshort    *readBuffer;
  int         readBufferSize;
  int         preloadedSamples;
  int         size;
  int         samplerate;
} audio_wav_DecoderData;


int audio_wav_load(unsigned int buffer, char const* filename);
