/*
#   clove
#
#   Copyright (C) 2018 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/

#include "graphics_geometry.h"
#include "../graphics/geometry.h"


static struct
{
  int currentDataSize;
  int* vertices;
} moduleData;


ar_Value* ar_geometry_circle(ar_State* S, ar_Value* args, ar_Value* env)
{
	const char* type = ar_eval_string(S, ar_car(args), env);
	float x = ar_eval_number(S, ar_nth(args, 1), env);
	float y = ar_eval_number(S, ar_nth(args, 2), env);
	float rad = ar_eval_number(S, ar_nth(args, 3), env);
	float s = ar_opt_number(S, ar_nth(args, 4), env, 12);
	float r = ar_opt_number(S, ar_nth(args, 5), env, 0);
	float sx = ar_opt_number(S, ar_nth(args, 6), env, 1.0f);
	float sy = ar_opt_number(S, ar_nth(args, 7), env, 1.0f);
	float ox = ar_opt_number(S, ar_nth(args, 8), env, 0);
	float oy = ar_opt_number(S, ar_nth(args, 9), env, 0);

	if (strcmp(type, "line") == 0)
		graphics_geometry_lineCircle(x, y, rad, s, r, sx, sy, ox, oy);
	else if (strcmp(type, "fill") == 0)
	   graphics_geometry_fillCircle(x, y, rad, s, r, sx, sy, ox, oy);

	return NULL;
}

ar_Value* ar_geometry_rectangle(ar_State* S, ar_Value* args, ar_Value* env)
{
	const char* type = ar_eval_string(S, ar_car(args), env);
	float x = ar_eval_number(S, ar_nth(args, 1), env);
	float y = ar_eval_number(S, ar_nth(args, 2), env);
	float w = ar_eval_number(S, ar_nth(args, 3), env);
	float h = ar_eval_number(S, ar_nth(args, 4), env);
	float r = ar_opt_number(S,  ar_nth(args, 5), env, 0);
	float sx = ar_opt_number(S, ar_nth(args, 6), env, 1.0f);
	float sy = ar_opt_number(S, ar_nth(args, 7), env, 1.0f);
	float ox = ar_opt_number(S, ar_nth(args, 8), env, 0);
	float oy = ar_opt_number(S, ar_nth(args, 9), env, 0);

	if (strcmp(type, "line") == 0)
		graphics_geometry_rectangle(false, x, y, w, h, r, sx, sy, ox, oy);
	else if(strcmp(type, "fill") == 0)
		graphics_geometry_rectangle(true, x, y, w, h, r, sx, sy, ox, oy);

	return NULL;
}


ar_Value* ar_geometry_points(ar_State* S, ar_Value* args, ar_Value* env)
{
	float x = ar_eval_number(S, ar_car(args), env);
	float y = ar_eval_number(S, ar_nth(args, 1), env);
	graphics_geometry_points(x, y);
	return NULL;
}


ar_Value* ar_geometry_vertex(ar_State* S, ar_Value* args, ar_Value* env)
{
	const char* type = ar_eval_string(S, ar_car(args), env);
	float x = ar_eval_number(S, ar_nth(args, 1), env);
	float y = ar_eval_number(S, ar_nth(args, 2), env);
	ar_Value* elements = ar_eval(S, ar_nth(args, 3), env);
	int count = ar_eval_number(S, ar_nth(args, 4), env);

	int data_size = sizeof(int) * count;

	if (moduleData.currentDataSize < data_size)
	{
		free(moduleData.vertices);
		moduleData.vertices = (int*)malloc(data_size);
		moduleData.currentDataSize = data_size;
	}
	if (moduleData.vertices == 0)
	{
		printf("Error: Could not allocate memory for ar_geometry_vertex \n");
		return NULL;
	}

	int i = 0;
	while (ar_car(elements) != NULL)
	{
		int v = (int)ar_eval_number(S, ar_car(elements), env);
		moduleData.vertices[i] = v;
		i++;
		elements = ar_cdr(elements);
	}

	if (strncmp(type,"line",4) == 0)
		graphics_geometry_vertex(false, x, y,moduleData.vertices,count);
	else if (strncmp(type, "fill",4) == 0)
		graphics_geometry_vertex(true, x, y, moduleData.vertices,count);
	return NULL;
}


void ar_geometry_register(ar_State* S)
{
	struct { const char* name; ar_Prim fn; } prims[] =
	{
		{"love:graphics-rectangle", ar_geometry_rectangle},
		{"love:graphics-circle", ar_geometry_circle},
		{"love:graphics-points", ar_geometry_points},
		{"love:graphics-vertex", ar_geometry_vertex},
		{NULL, NULL}
	};
	for (int i = 0; prims[i].name; i++)
		ar_bind_global(S, prims[i].name, ar_new_prim(S, prims[i].fn));
}






