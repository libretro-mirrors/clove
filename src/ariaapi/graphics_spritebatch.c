/*
#   clove
#
#   Copyright (C) 2018 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/
#include "graphics_spritebatch.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "../graphics/batch.h"
#include "../graphics/quad.h"
#include "../graphics/image.h"

static const graphics_Quad defaultQuad =
{
  .x = 0.0,
  .y = 0.0,
  .w = 1.0,
  .h = 1.0
};


ar_Value* ar_graphics_newSpriteBatch(ar_State* S, ar_Value* args, ar_Value* env)
{
	graphics_Image const* image = (graphics_Image*) ar_eval_udata(S,
			ar_car(args), env);

	int count = ar_opt_number(S, ar_nth(args, 1), env, 256);

	graphics_Batch* batch = malloc(sizeof(graphics_Batch));
	graphics_Batch_new(batch, image, count, graphics_BatchUsage_static);

	return ar_new_udata(S, batch);
}


ar_Value* ar_graphics_gcSpriteBatch(ar_State* S, ar_Value* args, ar_Value* env)
{
	graphics_Batch* batch = (graphics_Batch*) ar_eval_udata(S, ar_car(args), env);
	graphics_Batch_free(batch);
	return NULL;
}


ar_Value* ar_graphics_SpriteBatch_bind(ar_State* S, ar_Value* args, ar_Value* env)
{
	graphics_Batch* batch = (graphics_Batch*) ar_eval_udata(S, ar_car(args), env);
	graphics_Batch_bind(batch);
	return NULL;
}


ar_Value* ar_graphics_SpriteBatch_unbind(ar_State* S, ar_Value* args, ar_Value* env)
{
	graphics_Batch* batch = (graphics_Batch*) ar_eval_udata(S, ar_car(args), env);
	graphics_Batch_unbind(batch);
	return NULL;
}


ar_Value* ar_graphics_SpriteBatch_flush(ar_State* S, ar_Value* args, ar_Value* env)
{
	graphics_Batch* batch = (graphics_Batch*) ar_eval_udata(S, ar_car(args), env);
	graphics_Batch_flush(batch);
	return NULL;
}


ar_Value* ar_graphics_SpriteBatch_clear(ar_State* S, ar_Value* args, ar_Value* env)
{
	graphics_Batch* batch = (graphics_Batch*) ar_eval_udata(S, ar_car(args), env);
	graphics_Batch_clear(batch);
	return NULL;
}


ar_Value* ar_graphics_SpriteBatch_getBufferSize(ar_State* S, ar_Value* args, ar_Value* env)
{
	graphics_Batch* batch = (graphics_Batch*) ar_eval_udata(S, ar_car(args), env);
	return ar_new_number(S, batch->maxCount);
}


ar_Value* ar_graphics_SpriteBatch_setBufferSize(ar_State* S, ar_Value* args, ar_Value* env)
{
	graphics_Batch* batch = (graphics_Batch*) ar_eval_udata(S, ar_car(args), env);
	graphics_Batch_changeBufferSize(batch, ar_eval_number(S, ar_nth(args, 1), env));
	return NULL;
}


ar_Value* ar_graphics_SpriteBatch_setCount(ar_State* S, ar_Value* args, ar_Value* env)
{
	graphics_Batch* batch = (graphics_Batch*) ar_eval_udata(S, ar_car(args), env);
	return ar_new_number(S, batch->insertPos);
}


ar_Value* ar_graphics_SpriteBatch_setTexture(ar_State* S, ar_Value* args, ar_Value* env)
{
	graphics_Batch* batch = (graphics_Batch*) ar_eval_udata(S, ar_car(args), env);
	graphics_Image const* image = (graphics_Image*)
		ar_eval_udata(S, ar_nth(args, 1), env);
	batch->texture = image;
	return NULL;
}


ar_Value* ar_graphics_SpriteBatch_getTexture(ar_State* S, ar_Value* args, ar_Value* env)
{
	graphics_Batch* batch = (graphics_Batch*) ar_eval_udata(S, ar_car(args), env);
	return ar_new_udata(S, (graphics_Image*) batch->texture);
}


ar_Value* ar_graphics_SpriteBatch_setColor(ar_State* S, ar_Value* args, ar_Value* env)
{
	graphics_Batch* batch = (graphics_Batch*) ar_eval_udata(S, ar_car(args), env);
	if (strcmp(ar_eval_string(S, ar_nth(args, 1), env), "clear") == 0)
		graphics_Batch_clearColor(batch);
	else
	{
		float r = ar_eval_number(S, ar_nth(args, 1), env);
		float g = ar_eval_number(S, ar_nth(args, 2), env);
		float b = ar_eval_number(S, ar_nth(args, 3), env);
		float a = ar_opt_number(S, ar_nth(args, 4), env, 255.0f);
		graphics_Batch_setColor(batch, r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
	}
	return NULL;
}


ar_Value* ar_graphics_SpriteBatch_getColor(ar_State* S, ar_Value* args, ar_Value* env)
{
	graphics_Batch* batch = (graphics_Batch*) ar_eval_udata(S, ar_car(args), env);

	if (!batch->colorSet)
		return 0;
	return ar_new_list(S, 4,
			ar_new_number(S, floor(batch->color.x * 255)),
			ar_new_number(S, floor(batch->color.y * 255)),
			ar_new_number(S, floor(batch->color.z * 255)),
			ar_new_number(S, floor(batch->color.w * 255)));
}


ar_Value* ar_graphics_SpriteBatch_set(ar_State* S, ar_Value* args, ar_Value* env)
{
	graphics_Batch* batch = (graphics_Batch*) ar_eval_udata(S, ar_car(args), env);
	graphics_Quad const* quad = &defaultQuad;

	int id = ar_eval_number(S, ar_nth(args, 1),  env);
	float x = ar_eval_number(S, ar_nth(args, 2), env);
	float y = ar_eval_number(S, ar_nth(args, 3), env);
	float r = ar_opt_number(S, ar_nth(args, 4),  env, 0);
	float sx = ar_opt_number(S, ar_nth(args, 5), env, 1.0f);
	float sy = ar_opt_number(S, ar_nth(args, 6), env, 1.0f);
	float ox = ar_opt_number(S, ar_nth(args, 7), env, 0);
	float oy = ar_opt_number(S, ar_nth(args, 8), env, 0);
	float kx = ar_opt_number(S, ar_nth(args, 9), env, 0);
	float ky = ar_opt_number(S, ar_nth(args, 10),env, 0);
	graphics_Batch_set(batch, id, quad, x, y, r, sx, sy, ox, oy, kx, ky);
	return NULL;
}


ar_Value* ar_graphics_SpriteBatch_setQ(ar_State* S, ar_Value* args, ar_Value* env)
{
	graphics_Batch* batch = (graphics_Batch*) ar_eval_udata(S, ar_car(args), env);
	graphics_Quad const* quad = (graphics_Quad*) ar_eval_udata(S, ar_nth(args, 1), env);

	int id = ar_eval_number(S, ar_nth(args, 2), env);
	float x = ar_eval_number(S, ar_nth(args, 3), env);
	float y = ar_eval_number(S, ar_nth(args, 4), env);
	float r = ar_opt_number(S, ar_nth(args, 5), env, 0);
	float sx = ar_opt_number(S, ar_nth(args, 6), env, 1.0f);
	float sy = ar_opt_number(S, ar_nth(args, 7), env, 1.0f);
	float ox = ar_opt_number(S, ar_nth(args, 8), env, 0);
	float oy = ar_opt_number(S, ar_nth(args, 9), env, 0);
	float kx = ar_opt_number(S, ar_nth(args, 10), env, 0);
	float ky = ar_opt_number(S, ar_nth(args, 11), env, 0);
	graphics_Batch_set(batch, id, quad, x, y, r, sx, sy, ox, oy, kx, ky);
	return NULL;
}


ar_Value* ar_graphics_SpriteBatch_add(ar_State* S, ar_Value* args, ar_Value* env)
{
	graphics_Batch* batch = (graphics_Batch*) ar_eval_udata(S, ar_car(args), env);
	graphics_Quad const* quad = &defaultQuad;

	float x = ar_eval_number(S, ar_nth(args, 1), env);
	float y = ar_eval_number(S, ar_nth(args, 2), env);
	float r = ar_opt_number(S, ar_nth(args, 3),  env, 0);
	float sx = ar_opt_number(S, ar_nth(args, 4),  env, 1.0f);
	float sy = ar_opt_number(S, ar_nth(args, 5),  env, 1.0f);
	float ox = ar_opt_number(S, ar_nth(args, 6),  env, 0);
	float oy = ar_opt_number(S, ar_nth(args, 7),  env, 0);
	float kx = ar_opt_number(S, ar_nth(args, 8),  env, 0);
	float ky = ar_opt_number(S, ar_nth(args, 9),  env, 0);
	int i = graphics_Batch_add(batch, quad, x, y, r, sx, sy, ox, oy, kx, ky);
	return ar_new_number(S, i);
}


ar_Value* ar_graphics_SpriteBatch_addQ(ar_State* S, ar_Value* args, ar_Value* env)
{
	graphics_Batch* batch = (graphics_Batch*) ar_eval_udata(S, ar_car(args), env);
	graphics_Quad const* quad = (graphics_Quad*) ar_eval_udata(S, ar_nth(args, 1), env);

	float x = ar_eval_number(S, ar_nth(args, 2), env);
	float y = ar_eval_number(S, ar_nth(args, 3), env);
	float r = ar_opt_number(S, ar_nth(args, 4),  env, 0);
	float sx = ar_opt_number(S, ar_nth(args, 5), env, 1.0f);
	float sy = ar_opt_number(S, ar_nth(args, 6), env, 1.0f);
	float ox = ar_opt_number(S, ar_nth(args, 7), env, 0);
	float oy = ar_opt_number(S, ar_nth(args, 8), env, 0);
	float kx = ar_opt_number(S, ar_nth(args, 9), env, 0);
	float ky = ar_opt_number(S, ar_nth(args, 10), env, 0);
	int i = graphics_Batch_add(batch, quad, x, y, r, sx, sy, ox, oy, kx, ky);
	return ar_new_number(S, i);
}


ar_Value* ar_graphics_SpriteBatch_draw(ar_State* S, ar_Value* args, ar_Value* env)
{
	graphics_Batch* batch = (graphics_Batch*) ar_eval_udata(S, ar_car(args), env);
	graphics_Quad const* quad = &defaultQuad;

	float x = ar_opt_number(S, ar_nth(args, 1), env, 0);
	float y = ar_opt_number(S, ar_nth(args, 2), env, 0);
	float r = ar_opt_number(S, ar_nth(args, 3),  env, 0);
	float sx = ar_opt_number(S, ar_nth(args, 4), env, 1.0f);
	float sy = ar_opt_number(S, ar_nth(args, 5), env, 1.0f);
	float ox = ar_opt_number(S, ar_nth(args, 6), env, 0);
	float oy = ar_opt_number(S, ar_nth(args, 7), env, 0);
	float kx = ar_opt_number(S, ar_nth(args, 8), env, 0);
	float ky = ar_opt_number(S, ar_nth(args, 9), env, 0);
	graphics_Batch_draw(batch, x, y, r, sx, sy, ox, oy, kx, ky);
	return NULL;
}


void ar_graphics_SpriteBatch_register(ar_State* S)
{
	struct { const char* name; ar_Prim fn; } prims[] =
	{
		{"love:graphics-newSpriteBatch", ar_graphics_newSpriteBatch},
		{"love:graphics-drawSpriteBatch", ar_graphics_SpriteBatch_draw},
		{"love:graphics-spritebatch-free", ar_graphics_gcSpriteBatch},
		{"love:graphics-spritebatch-bind", ar_graphics_SpriteBatch_bind},
		{"love:graphics-spritebatch-unbind", ar_graphics_SpriteBatch_unbind},
		{"love:graphics-spritebatch-flush", ar_graphics_SpriteBatch_flush},
		{"love:graphics-spritebatch-clear", ar_graphics_SpriteBatch_clear},
		{"love:graphics-spritebatch-getBufferSize", ar_graphics_SpriteBatch_getBufferSize},
		{"love:graphics-spritebatch-setBufferSize", ar_graphics_SpriteBatch_setBufferSize},
		{"love:graphics-spritebatch-setCount", ar_graphics_SpriteBatch_setCount},
		{"love:graphics-spritebatch-setTexture", ar_graphics_SpriteBatch_setTexture},
		{"love:graphics-spritebatch-getTexture", ar_graphics_SpriteBatch_getTexture},
		{"love:graphics-spritebatch-setColor", ar_graphics_SpriteBatch_setColor},
		{"love:graphics-spritebatch-getColor", ar_graphics_SpriteBatch_getColor},
		{"love:graphics-spritebatch-set", ar_graphics_SpriteBatch_set},
		{"love:graphics-spritebatch-setq", ar_graphics_SpriteBatch_setQ},
		{"love:graphics-spritebatch-add", ar_graphics_SpriteBatch_add},
		{"love:graphics-spritebatch-addq", ar_graphics_SpriteBatch_addQ},
		{NULL, NULL}
	};
	for (int i = 0; prims[i].name; i++)
		ar_bind_global(S, prims[i].name, ar_new_prim(S, prims[i].fn));
}


