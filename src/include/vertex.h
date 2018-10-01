/*
#   clove
#
#   Copyright (C) 2016-2018 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/
#pragma once

#include "vector.h"

typedef struct {
  vec2 pos;
  vec2 uv;
  vec4 color;
} graphics_Vertex;

typedef struct {
  vec3 pos;
  vec2 uv;
  vec4 color;
} graphics_Vertex3d;
