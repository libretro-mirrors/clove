/*
#   clove
#
#   Copyright (C) 2018 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/
#include "graphics.h"

#include "../graphics/graphics.h"
#include "../graphics/matrixstack.h"
#include "../graphics/shader.h"
#include "../graphics/gltools.h"

ar_Value* ar_graphics_clear(ar_State* S, ar_Value* args, ar_Value* env)
{
	graphics_clear();
	return NULL;
}


ar_Value* ar_graphics_swap(ar_State* S, ar_Value* args, ar_Value* env)
{
	graphics_swap();
	return NULL;
}


ar_Value* ar_graphics_push(ar_State* S, ar_Value* args, ar_Value* env)
{
	if (matrixstack_push())
		clove_error("Matrix stack overflow\n");
	return NULL;
}


ar_Value* ar_graphics_pop(ar_State* S, ar_Value* args, ar_Value* env)
{
	if (matrixstack_pop())
		clove_error("Matrix stack underflow\n");
	return NULL;
}


ar_Value* ar_graphics_origin(ar_State* S, ar_Value* args, ar_Value* env)
{
	matrixstack_origin();
	return NULL;
}


ar_Value* ar_graphics_getWidth(ar_State* S, ar_Value* args, ar_Value* env)
{
	return ar_new_number(S, graphics_getWidth());
}


ar_Value* ar_graphics_getHeight(ar_State* S, ar_Value* args, ar_Value* env)
{
	return ar_new_number(S, graphics_getHeight());
}


ar_Value* ar_graphics_reset(ar_State* S, ar_Value* args, ar_Value* env)
{
	graphics_reset();
	return NULL;
}


ar_Value* ar_graphics_translate(ar_State* S, ar_Value* args, ar_Value* env)
{
	float x = ar_eval_number(S, ar_car(args), env);
	float y = ar_eval_number(S, ar_nth(args, 1), env);
	float z = ar_eval_number(S, ar_nth(args, 2), env);

	matrixstack_translate(x, y, z);
	return NULL;
}


ar_Value* ar_graphics_scale(ar_State* S, ar_Value* args, ar_Value* env)
{
	float x = ar_eval_number(S, ar_car(args), env);
	float y = ar_eval_number(S, ar_nth(args, 1), env);
	float z = ar_eval_number(S, ar_nth(args, 2), env);

	matrixstack_scale(x, y, z);
	return NULL;
}


ar_Value* ar_graphics_rotate(ar_State* S, ar_Value* args, ar_Value* env)
{
	float r = ar_eval_number(S, ar_car(args), env);

	matrixstack_rotate(r);
	return NULL;
}


ar_Value* ar_graphics_setBackgroundColor(ar_State* S, ar_Value* args, ar_Value* env)
{
	int red   = ar_eval_number(S, ar_car(args), env);
	int green   = ar_eval_number(S, ar_nth(args, 1), env);
	int blue   = ar_eval_number(S, ar_nth(args, 2), env);
	int alpha   = ar_eval_number(S, ar_nth(args, 3), env);

    float scale = 1.0f / 255.0f;

    graphics_setBackgroundColor(red * scale, green * scale, blue * scale, alpha * scale);
	return NULL;
}


ar_Value* ar_graphics_getBackgroundColor(ar_State* S, ar_Value* args, ar_Value* env)
{
	float* colors = graphics_getBackgroundColor();

	return ar_new_list(S, 4,
			ar_new_number(S, (int)colors[0]*255),
			ar_new_number(S, (int)colors[1]*255),
			ar_new_number(S, (int)colors[2]*255),
			ar_new_number(S, (int)colors[3]*255));
}


ar_Value* ar_graphics_setColor(ar_State* S, ar_Value* args, ar_Value* env)
{
	int red   = ar_eval_number(S, ar_car(args), env);
	int green   = ar_eval_number(S, ar_nth(args, 1), env);
	int blue   = ar_eval_number(S, ar_nth(args, 2), env);
	int alpha   = ar_eval_number(S, ar_nth(args, 3), env);

    float scale = 1.0f / 255.0f;

    graphics_setColor(red * scale, green * scale, blue * scale, alpha * scale);
	return NULL;
}


ar_Value* ar_graphics_setScissor(ar_State* S, ar_Value* args, ar_Value* env)
{

	if (ar_type(ar_eval(S, ar_car(args), env)) == AR_TNIL)
	{
		graphics_clearScissor();
		return NULL;
	}

	int x   = ar_eval_number(S, ar_car(args), env);
	int y   = ar_eval_number(S, ar_nth(args, 1), env);
	int w   = ar_eval_number(S, ar_nth(args, 2), env);
	int h   = ar_eval_number(S, ar_nth(args, 3), env);
	graphics_setScissor(x,y,w,h);
	return NULL;
}


ar_Value* ar_graphics_getScissor(ar_State* S, ar_Value* args, ar_Value* env)
{
	int x, y, w, h;
	bool scissor = graphics_getScissor(&x, &y, &w, &h);
	if (!scissor)
		return NULL;

	return ar_new_list(S, 4,
			ar_new_number(S, x),
			ar_new_number(S, y),
			ar_new_number(S, w),
			ar_new_number(S, h));
}


//TODO add blend more


ar_Value* ar_graphics_update(ar_State* S, ar_Value* args, ar_Value* env)
{
	graphics_clear();
	matrixstack_origin();
	return NULL;
}


void ar_graphics_register(ar_State* S)
{
	struct { const char* name; ar_Prim fn; } prims[] =
	{
		{"love:graphics-setBackgroundColor", ar_graphics_setBackgroundColor},
		{"love:graphics-getBackgroundColor", ar_graphics_getBackgroundColor},
		{"love:graphics-setColor", ar_graphics_setColor},
		{"love:graphics-setScissor", ar_graphics_setScissor},
		{"love:graphics-getScissor", ar_graphics_getScissor},
		{"love:graphics-clear", ar_graphics_clear},
		{"love:graphics-origin", ar_graphics_origin},
		{"love:graphics-translate", ar_graphics_translate},
		{"love:graphics-rotate", ar_graphics_rotate},
		{"love:graphics-scale", ar_graphics_scale},
		{"love:graphics-pop", ar_graphics_pop},
		{"love:graphics-push", ar_graphics_push},
		{"love:graphics-swap", ar_graphics_swap},
		{"love:graphics-getWidth", ar_graphics_getWidth},
		{"love:graphics-getHeight", ar_graphics_getHeight},
		{"love:graphics-reset", ar_graphics_reset},
		{"love:graphics-update", ar_graphics_update},
		{NULL, NULL}
	};
	for (int i = 0; prims[i].name; i++)
		ar_bind_global(S, prims[i].name, ar_new_prim(S, prims[i].fn));
}
