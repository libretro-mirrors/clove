/*
#   clove
#
#   Copyright (C) 2016-2017 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/
#pragma once

#include "../3rdparty/lua/lua.h"
#include "../audio/audio.h"

#include "../audio/staticsource.h"
#include "../audio/streamsource.h"

int l_audio_register(lua_State *state);
bool l_audio_isStaticSource(lua_State* state, int index);
bool l_audio_isStreamSource(lua_State* state, int index);
