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
#include "../graphics/quad.h"
#include <stdbool.h>

int l_graphics_newQuad(lua_State* state);
void l_graphics_quad_register(lua_State* state);
bool l_graphics_isQuad(lua_State* state, int index);
graphics_Quad* l_graphics_toQuad(lua_State* state, int index);

