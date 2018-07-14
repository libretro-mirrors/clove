/*
#   clove
#
#   Copyright (C) 2018 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "../graphics/mesh.h"
#include "../tools/utils.h"

#include "graphics_image.h"

static struct
{
  graphics_Vertex* vertices;
  unsigned int* indices;
  size_t verticesSize;
  size_t indicesSize;
} moduleData;


static void feedVertices(int size)
{
  if (moduleData.verticesSize <  size)
    {
      free(moduleData.vertices);
      moduleData.vertices = malloc(size);
      moduleData.verticesSize = size;
    }
}


static void feedIndices(int size)
{
  if (moduleData.indicesSize <  size)
    {
      free(moduleData.indices);
      moduleData.indices= malloc(size);
      moduleData.indicesSize = size;
    }
}


static int readVertices(ar_State* S, ar_Value* args, ar_Value* env, int index)
{
  ar_Value* pair = NULL;
  if (index == 0)
    pair = ar_eval(S, ar_car(args), env);
  else
    pair = ar_eval(S, ar_nth(args, index), env);
  int count = ar_length(pair) / 8;
  if (count % 8 != 0)
    clove_error("Your number of vertices is not right in your mesh! %d \n", count);
  feedVertices(count * sizeof(graphics_Vertex));

  int c = 0;
  for (int i = 0; i < count; ++i)
    {
      if (c == 0)
        moduleData.vertices[i].pos.x = ar_eval_number(S, ar_car(pair), env);
      else
        {
          c++;
          moduleData.vertices[i].pos.x = ar_eval_number(S, ar_nth(pair, c), env);
        }
      c++;
      moduleData.vertices[i].pos.y = ar_eval_number(S, ar_nth(pair, c), env);
      c++;
      moduleData.vertices[i].uv.x = ar_eval_number(S, ar_nth(pair, c), env);
      c++;
      moduleData.vertices[i].uv.y = ar_eval_number(S, ar_nth(pair, c), env);
      c++;
      moduleData.vertices[i].color.x = ar_eval_number(S, ar_nth(pair, c), env) / 255.0f;
      c++;
      moduleData.vertices[i].color.y = ar_eval_number(S, ar_nth(pair, c), env) / 255.0f;
      c++;
      moduleData.vertices[i].color.z = ar_eval_number(S, ar_nth(pair, c), env) / 255.0f;
      c++;
      moduleData.vertices[i].color.w = ar_eval_number(S, ar_nth(pair, c), env) / 255.0f;
    }
  return count;
}


static int readIndices(ar_State* S, ar_Value* args, ar_Value* env, int index)
{
  ar_Value* pair = NULL;
  if (index == 0)
    pair = ar_eval(S, ar_car(args), env);
  else
    pair = ar_eval(S, ar_nth(args, index), env);

  int count = ar_length(pair);
  feedIndices(count * sizeof(unsigned int));

  for (int i = 0; i < count; i++)
    {
      if (i == 0)
        moduleData.indices[i] = ar_eval_number(S, ar_car(pair), env);
      else
        moduleData.indices[i] = ar_eval_number(S, ar_nth(pair, i), env);
    }


  return count;
}


ar_Value* ar_graphics_newMesh(ar_State* S, ar_Value* args, ar_Value* env)
{
  int vertexCount = readVertices(S, args, env, 0);
  int indices = readIndices(S, args, env, 1);
  const char* str_mode = ar_eval_string(S, ar_nth(args, 2), env);
  graphics_MeshDrawMode3d mode = graphics_MeshDrawMode_fan;

  if (strcmp(str_mode, "fan") == 0)
    mode = graphics_MeshDrawMode_fan;
  else if (strcmp(str_mode, "strip") == 0)
    mode = graphics_MeshDrawMode_strip;
  else if (strcmp(str_mode, "triangles") == 0)
    mode = graphics_MeshDrawMode_triangles;
  else if (strcmp(str_mode, "points") == 0)
    mode = graphics_MeshDrawMode_points;
  else
    {
      clove_error("Can't handle mesh mode: %s", str_mode);
      return NULL;
    }
  graphics_Mesh3d* mesh = malloc(sizeof(graphics_Mesh3d));
  graphics_Mesh_new(mesh, vertexCount, moduleData.vertices, indices,
                    moduleData.indices, mode);
  return ar_new_udata(S, mesh);
}


ar_Value* ar_graphics_drawMesh(ar_State* S, ar_Value* args, ar_Value* env)
{
  graphics_Mesh3d* mesh = (graphics_Mesh3d*)ar_eval_udata(S, ar_car(args), env);
  float x = ar_eval_number(S, ar_nth(args, 1), env);
  float y = ar_eval_number(S, ar_nth(args, 2), env);
  float r = ar_opt_number(S, ar_nth(args, 3), env, 0);
  float sx = ar_opt_number(S, ar_nth(args, 4), env, 1.0f);
  float sy = ar_opt_number(S, ar_nth(args, 5), env, 1.0f);
  float ox = ar_opt_number(S, ar_nth(args, 6), env, 0);
  float oy = ar_opt_number(S, ar_nth(args, 7), env, 0);
  float kx = ar_opt_number(S, ar_nth(args, 8), env, 0);
  float ky = ar_opt_number(S, ar_nth(args, 9), env, 0);

  graphics_Mesh_draw(mesh, x, y, r, sx, sy, ox, oy, kx, ky);
  return NULL;
}


ar_Value* ar_graphics_mesh_setTexture(ar_State* S, ar_Value* args, ar_Value* env)
{
  graphics_Mesh3d* mesh = (graphics_Mesh3d*) ar_eval_udata(S, ar_car(args), env);
  graphics_Image* image = (graphics_Image*) ar_eval_udata(S, ar_nth(args, 1), env);
  graphics_Mesh_setTexture(mesh, image);
  return NULL;
}


ar_Value* ar_graphics_mesh_setVertices(ar_State* S, ar_Value* args, ar_Value* env)
{
  graphics_Mesh3d* mesh = (graphics_Mesh3d*) ar_eval_udata(S, ar_car(args), env);
  int vertexCount = readVertices(S, args, env, 1);
  graphics_Mesh_setVertices(mesh, moduleData.vertices, vertexCount);
  return NULL;
}


ar_Value* ar_graphics_mesh_setIndices(ar_State* S, ar_Value* args, ar_Value* env)
{
  graphics_Mesh3d* mesh = (graphics_Mesh3d*) ar_eval_udata(S, ar_car(args), env);
  int indexCount = readIndices(S, args, env, 1);
  graphics_Mesh_setIndices(mesh, moduleData.indices, indexCount);
  return NULL;
}


ar_Value* ar_graphics_gcMesh(ar_State* S, ar_Value* args, ar_Value* env)
{
  graphics_Mesh3d* mesh = (graphics_Mesh3d*) ar_eval_udata(S, ar_car(args), env);
  graphics_Mesh_free(mesh);
  return NULL;
}


void ar_graphics_mesh_register(ar_State* S)
{
  struct { const char* name; ar_Prim fn; } prims[] =
  {
  {"love:graphics-newMesh", 					ar_graphics_newMesh},
  {"love:graphics-mesh-setTexture", 			        ar_graphics_mesh_setTexture},
  {"love:graphics-mesh-setVertices", 			        ar_graphics_mesh_setVertices},
  {"love:graphics-mesh-setIndices", 			        ar_graphics_mesh_setIndices},
  {"love:graphics-drawMesh", 					ar_graphics_drawMesh},
  {"love:graphics-mesh-free", 					ar_graphics_gcMesh},
  {NULL, NULL}
};
  for (int i = 0; prims[i].name; i++)
    ar_bind_global(S, prims[i].name, ar_new_prim(S, prims[i].fn));
}
