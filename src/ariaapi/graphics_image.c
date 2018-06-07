/*
#   clove
#
#   Copyright (C) 2018 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/
#include "graphics_image.h"

#include <stdlib.h>

#include "../graphics/image.h"
#include "../image/imagedata.h"

ar_Value* ar_graphics_newImage(ar_State* S, ar_Value* args, ar_Value* env)
{
	const char* filename = ar_eval_string(S, ar_car(args), env);

	graphics_Image* img = malloc(sizeof(graphics_Image));
	img->path = filename;
	image_ImageData image_data;

	int err = image_ImageData_new_with_filename(&image_data, filename);
	if (err == 0)
	{
		ar_error_str(S, "Could not find image %s\n", filename);
		return NULL;
	}
	graphics_Image_new_with_ImageData(img, &image_data);

	return ar_new_udata(S, img);
}


ar_Value* ar_graphics_Image_getPath(ar_State* S, ar_Value* args, ar_Value* env)
{
	graphics_Image* obj = (graphics_Image*)ar_eval_udata(S, ar_car(args), env);
	return ar_new_string(S, obj->path);
}


ar_Value* ar_graphics_Image_getDimensions(ar_State* S, ar_Value* args, ar_Value* env)
{
	graphics_Image* obj = (graphics_Image*)ar_eval_udata(S, ar_car(args), env);
	return ar_new_pair(S, ar_new_number(S, obj->width), ar_new_number(S, obj->height));
}


ar_Value* ar_graphics_Image_getWidth(ar_State* S, ar_Value* args, ar_Value* env)
{
	graphics_Image* obj = (graphics_Image*)ar_eval_udata(S, ar_car(args), env);
	return ar_new_number(S, obj->width);
}


ar_Value* ar_graphics_Image_getHeight(ar_State* S, ar_Value* args, ar_Value* env)
{
	graphics_Image* obj = (graphics_Image*)ar_eval_udata(S, ar_car(args), env);
	return ar_new_number(S, obj->height);
}


ar_Value* ar_graphics_Image_free(ar_State* S, ar_Value* args, ar_Value* env)
{
	graphics_Image* obj = (graphics_Image*)ar_eval_udata(S, ar_car(args), env);
	graphics_Image_free(obj);
	free(obj);
	return NULL;
}


ar_Value* ar_graphics_Image_getWrap(ar_State* S, ar_Value* args, ar_Value* env)
{
	graphics_Image* obj = (graphics_Image*)ar_eval_udata(S, ar_car(args), env);

    graphics_Wrap wrap;
    graphics_Image_getWrap(obj, &wrap);

	const char* verMode = wrap.verMode == graphics_WrapMode_clamp ? "clamp" : "repeat";
	const char* horMode = wrap.horMode == graphics_WrapMode_clamp ? "clamp" : "repeat";

	return ar_new_pair(S, ar_new_string(S, horMode), ar_new_string(S, verMode));
}


ar_Value* ar_graphics_Image_setWrap(ar_State* S, ar_Value* args, ar_Value* env)
{
	graphics_Image* obj = (graphics_Image*)ar_eval_udata(S, ar_car(args), env);

	graphics_Wrap wrap;
	const char* horMode = ar_eval_string(S, ar_nth(args, 1), env);
	const char* verMode = ar_eval_string(S, ar_nth(args, 2), env);

	if (strcmp(horMode, "clamp") == 0)
		wrap.horMode = graphics_WrapMode_clamp;
	else if (strcmp(horMode, "repeat") == 0)
		wrap.horMode = graphics_WrapMode_repeat;

	if (strcmp(verMode, "clamp") == 0)
		wrap.verMode = graphics_WrapMode_clamp;
	else if (strcmp(verMode, "repeat") == 0)
		wrap.verMode = graphics_WrapMode_repeat;

	graphics_Image_setWrap(obj, &wrap);
	return NULL;
}


ar_Value* ar_graphics_Image_getFilter(ar_State* S, ar_Value* args, ar_Value* env)
{
	graphics_Image* obj = (graphics_Image*)ar_eval_udata(S, ar_car(args), env);

	graphics_Filter filter;
	graphics_Image_getFilter(obj, &filter);

	const char *minMode, *magMode;
	if (filter.minMode == graphics_FilterMode_linear)
		minMode = "linear";
	else if (filter.minMode == graphics_FilterMode_nearest)
		minMode = "nearest";

	if (filter.magMode == graphics_FilterMode_linear)
		magMode = "linear";
	else if (filter.magMode == graphics_FilterMode_nearest)
		magMode = "nearest";

	return ar_new_list(S, 3, ar_new_string(S, minMode), ar_new_string(S, magMode), ar_new_number(S, filter.maxAnisotropy));
}


ar_Value* ar_graphics_Image_setFilter(ar_State* S, ar_Value* args, ar_Value* env)
{
	graphics_Image* obj = (graphics_Image*)ar_eval_udata(S, ar_car(args), env);

    graphics_Filter newFilter;
    graphics_Image_getFilter(obj, &newFilter);

	const char *minMode = ar_eval_string(S, ar_nth(args, 1), env);
	const char *magMode = ar_eval_string(S, ar_nth(args, 2), env);

	if (strcmp(minMode, "linear") == 0)
		newFilter.minMode = graphics_FilterMode_linear;
	else if (strcmp(minMode, "nearest") == 0)
		newFilter.minMode = graphics_FilterMode_nearest;

	if (strcmp(magMode, "linear") == 0)
		newFilter.magMode = graphics_FilterMode_linear;
	else if (strcmp(magMode, "nearest") == 0)
		newFilter.magMode = graphics_FilterMode_nearest;

    newFilter.maxAnisotropy = ar_eval_number(S, ar_nth(args, 3), env);
    graphics_Image_setFilter(obj, &newFilter);
    return NULL;
}

//TODO add getMipmapFilter, setMipmapFilter

ar_Value* ar_graphics_Image_refresh(ar_State* S, ar_Value* args, ar_Value* env)
{
	graphics_Image* obj = (graphics_Image*)ar_eval_udata(S, ar_car(args), env);
	image_ImageData* data = (image_ImageData*)ar_eval_udata(S, ar_nth(args, 1), env);

	graphics_Image_refresh(obj, data);
	return NULL;
}


static const graphics_Quad defaultQuad = {
    .x = 0.0f,
    .y = 0.0f,
    .w = 1.0f,
    .h = 1.0f
};


ar_Value* ar_graphics_Image_draw(ar_State* S, ar_Value* args, ar_Value* env)
{
	graphics_Image* image = (graphics_Image*)ar_eval_udata(S, ar_car(args), env);
	float x = ar_eval_number(S, ar_nth(args, 1), env);
	float y = ar_eval_number(S, ar_nth(args, 2), env);
	float r = ar_opt_number(S, ar_nth(args, 3), env, 0);
	float sx = ar_opt_number(S, ar_nth(args, 4), env, 1);
	float sy = ar_opt_number(S, ar_nth(args, 5), env, 1);
	float ox = ar_opt_number(S, ar_nth(args, 6), env, 0);
	float oy = ar_opt_number(S, ar_nth(args, 7), env, 0);
	float kx = ar_opt_number(S, ar_nth(args, 8), env, 0);
	float ky = ar_opt_number(S, ar_nth(args, 9), env, 0);

	graphics_Image_draw(image, &defaultQuad, x, y, r, sx, sy, ox, oy, kx, ky);
	return NULL;
}


ar_Value* ar_graphics_Image_draw_quad(ar_State* S, ar_Value* args, ar_Value* env)
{
	graphics_Image* image = (graphics_Image*)ar_eval_udata(S, ar_car(args), env);
	graphics_Quad* quad = (graphics_Quad*)ar_eval_udata(S, ar_nth(args, 1), env);
	float x = ar_eval_number(S, ar_nth(args, 2), env);
	float y = ar_eval_number(S, ar_nth(args, 3), env);
	float r = ar_opt_number(S, ar_nth(args, 4), env, 0);
	float sx = ar_opt_number(S, ar_nth(args, 5), env, 1.0f);
	float sy = ar_opt_number(S, ar_nth(args, 6), env, 1.0f);
	float ox = ar_opt_number(S, ar_nth(args, 7), env, 0);
	float oy = ar_opt_number(S, ar_nth(args, 8), env, 0);
	float kx = ar_opt_number(S, ar_nth(args, 9), env, 0);
	float ky = ar_opt_number(S, ar_nth(args, 10), env, 0);

	graphics_Image_draw(image, quad, x, y, r, sx, sy, ox, oy, kx, ky);
	return NULL;
}


void ar_graphics_image_register(ar_State* S)
{
	struct { const char* name; ar_Prim fn; } prims[] =
	{
		{"love:graphics-newImage", ar_graphics_newImage},
		{"love:graphics-drawImage", ar_graphics_Image_draw},
		{"love:graphics-drawQuad", ar_graphics_Image_draw_quad},
		{"love:graphics-freeImage", ar_graphics_Image_free},
		{"love:graphics-image-getDimensions", ar_graphics_Image_getDimensions},
		{"love:graphics-image-getWidth", ar_graphics_Image_getWidth},
		{"love:graphics-image-getHeight", ar_graphics_Image_getHeight},
		{"love:graphics-image-setFilter", ar_graphics_Image_setFilter},
		{"love:graphics-image-getFilter", ar_graphics_Image_getFilter},
		{"love:graphics-image-setWrap", ar_graphics_Image_setWrap},
		{"love:graphics-image-getWrap", ar_graphics_Image_getWrap},
		{"love:graphics-image-refresh", ar_graphics_Image_refresh},
		{"love:graphics-image-getPath", ar_graphics_Image_getPath},
		{NULL, NULL}
	};

	for (int i = 0; prims[i].name; i++)
		ar_bind_global(S, prims[i].name, ar_new_prim(S, prims[i].fn));
}
