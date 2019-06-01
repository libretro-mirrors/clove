/*
#   clove
#
#   Copyright (C) 2017-2019 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/
#pragma once

#include "gl.h"

#define DEFAULT_SAMPLER "tex"

/********* 2d default shader ************/

static GLchar const defaultVertexSource[] =
"vec4 position(mat4 transform_projection, vec4 vertex_position) {\n"
"  return transform_projection * vertex_position;\n"
"}\n";

static GLchar const vertexHeader[] =
#ifndef CLOVE_WEB
"#version 120\n"
#endif
"uniform   mat4 projection;\n"
"uniform   mat4 view;\n"
"uniform   mat4 model;\n"
"uniform   mat2 textureRect;\n"
"uniform   vec2 size;\n"
"#define extern uniform\n"
"attribute vec2 vPos;\n"
"attribute vec2 vUV;\n"
"attribute vec4 vColor;\n"
"varying   vec2 fUV;\n"
"varying   vec4 fColor;\n"
"#line 0\n";

static GLchar const vertexFooter[] =
"void main() {\n"
"  gl_Position = position(projection * view * model, vec4(vPos * size, 1.0, 1.0));\n"
"  fUV = vUV * textureRect[1] + textureRect[0];\n"
"  fColor = vColor;\n"
"}\n";

static GLchar const defaultFragmentSource[] =
"vec4 effect( vec4 color, Image texture, vec2 texture_coords, vec2 screen_coords ) {\n"
"  return Texel(texture, texture_coords) * color;\n"
"}\n";

static GLchar const fragmentHeader[] =
#ifndef CLOVE_WEB
"#version 120\n"
#endif
#ifdef CLOVE_WEB
"precision mediump float;\n"
#endif
"#define Image sampler2D\n"
"#define Texel texture2D\n"
"#define extern uniform\n"
"varying vec2 fUV;\n"
"varying vec4 fColor;\n"
"uniform sampler2D " DEFAULT_SAMPLER ";\n"
"uniform vec4 color;\n"
"#line 0\n";

static GLchar const fragmentFooter[] =
"void main() {\n"
"  gl_FragColor = effect(color * fColor, tex, fUV, vec2(0.0, 0.0));\n"
"}\n";


/********* end of 2d default shader ************/

