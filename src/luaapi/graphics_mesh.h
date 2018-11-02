/*
#   clove
#
#   Copyright (C) 2017-2018 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/
#pragma once

#ifdef USE_LUA

#include <stdbool.h>

#include "../3rdparty/lua/lua.h"

#include "tools.h"
#include "graphics_image.h"
#include "image.h"

#include "../include/mesh.h"

typedef struct {
    graphics_Mesh3d mesh;
    int textureRef;
}   l_graphics_Mesh;

void l_graphics_mesh_register(lua_State* state);
bool l_graphics_isMesh(lua_State* state, int index);
l_graphics_Mesh* l_graphics_toMesh(lua_State* state, int index);
int l_graphics_newMesh(lua_State* state);

#endif
