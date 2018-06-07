/*
#   clove
#
#   Copyright (C) 2018 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/

#include "graphics_quad.h"

#include "../graphics/quad.h"


ar_Value* ar_graphics_newQuad(ar_State* S, ar_Value* args, ar_Value* env)
{

	float x = ar_eval_number(S, ar_car(args), env);
	float y = ar_eval_number(S, ar_nth(args, 1), env);
	float w = ar_eval_number(S, ar_nth(args, 2), env);
	float h = ar_eval_number(S, ar_nth(args, 3), env);
	float rw = ar_eval_number(S, ar_nth(args, 4), env);
	float rh = ar_eval_number(S, ar_nth(args, 5), env);

	graphics_Quad* quad = malloc(sizeof(graphics_Quad));
	graphics_Quad_newWithRef(quad, x, y, w, h, rw, rh);

	return ar_new_udata(S, quad);
}


ar_Value* ar_graphics_freeQuad(ar_State* S, ar_Value* args, ar_Value* env)
{
	graphics_Quad* quad = (graphics_Quad*)ar_eval_udata(S, ar_car(args), env);
	free(quad);
	return NULL;
}


ar_Value* ar_graphics_Quad_getViewport(ar_State* S, ar_Value* args, ar_Value* env)
{
	graphics_Quad* quad = (graphics_Quad*)ar_eval_udata(S, ar_car(args), env);
	return ar_new_list(S, 4,
			ar_new_number(S, quad->x),
			ar_new_number(S, quad->y),
			ar_new_number(S, quad->w),
			ar_new_number(S, quad->h));
}


ar_Value* ar_graphics_Quad_setViewport(ar_State* S, ar_Value* args, ar_Value* env)
{
	graphics_Quad* quad = (graphics_Quad*)ar_eval_udata(S, ar_car(args), env);

	float x = ar_eval_number(S, ar_nth(args, 1), env);
	float y = ar_eval_number(S, ar_nth(args, 2), env);
	float w = ar_eval_number(S, ar_nth(args, 3), env);
	float h = ar_eval_number(S, ar_nth(args, 4), env);

	quad->x = x;
	quad->y = y;
	quad->w = w;
	quad->h = h;

	return NULL;
}


void ar_graphics_quad_register(ar_State* S)
{
	struct { const char* name; ar_Prim fn; } prims[] =
	{
		{"love:graphics-newQuad", ar_graphics_newQuad},
		{"love:graphics-quad-gc", ar_graphics_freeQuad},
		{"love:graphics-quad-setViewport", ar_graphics_Quad_setViewport},
		{"love:graphics-quad-getViewport", ar_graphics_Quad_getViewport},
		{NULL, NULL}
	};
	for (int i = 0; prims[i].name; i++)
		ar_bind_global(S, prims[i].name, ar_new_prim(S, prims[i].fn));
}


