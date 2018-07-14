/*
#   clove
#
#   Copyright (C) 2018 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/
#include "graphics_font.h"

#include "../graphics/font.h"
#include "../graphics/matrixstack.h"
#include "../graphics/shader.h"
#include "../graphics/graphics.h"

static struct
{
  graphics_Font* currentFont;
  graphics_Font defaultFont;
} moduleData;


void ar_graphics_loadDefaultFont()
{
  graphics_Font_new(&moduleData.defaultFont, NULL, 12);
  moduleData.currentFont = &moduleData.defaultFont;
}


ar_Value* ar_graphics_newFont(ar_State* S, ar_Value* args, ar_Value* env)
{
  char const* filename = NULL;
  int ptsize = 12;

  ar_Value* arg = ar_eval(S, ar_car(args), env);

  if (ar_type(arg) != AR_TNIL)
    {
      if (ar_type(arg) == AR_TSTRING)
        {
          filename = ar_eval_string(S, ar_car(args), env);
          if (ar_type(ar_eval(S, ar_nth(args, 1), env)) == AR_TNUMBER)
            ptsize = ar_eval_number(S, ar_nth(args, 1), env);
          else
            ptsize = 12;
        }
      else if (ar_type(arg) == AR_TNUMBER)
        {
          ptsize = ar_eval_number(S, ar_car(args), env);
        }
    }

  graphics_Font* font = malloc(sizeof(graphics_Font));
  if (filename)
    graphics_Font_new(font, filename, ptsize);
  else
    graphics_Font_new(font, NULL, ptsize);

  return ar_new_udata(S, font);
}


ar_Value* ar_graphics_setFont(ar_State* S, ar_Value* args, ar_Value* env)
{
  graphics_Font* font;

  if (ar_length(args) == 0) {
      clove_error("You must provide a font size or a font for setFont! \n");
      return NULL;
    }

  ar_Value* arg = ar_eval(S, ar_car(args), env);
  if (ar_type(arg) == AR_TNUMBER)
    {
      font = &moduleData.defaultFont;
      graphics_Font_new(font, NULL, arg->u.num.n);
    }
  else if (ar_type(arg) == AR_TUDATA)
    {
      font = (graphics_Font*) arg->u.udata.ptr;
      int size = ar_opt_number(S, ar_nth(args, 1), env, 12);
      graphics_Font_new(font, NULL, size);
    }

  return ar_new_udata(S, font);
}


ar_Value* ar_graphics_Font_getWidth(ar_State* S, ar_Value* args, ar_Value* env)
{
  graphics_Font* font = (graphics_Font*)ar_eval_udata(S, ar_car(args), env);

  char const* line = ar_eval_string(S, ar_nth(args, 1), env);
  int width = graphics_Font_getWidth(font, line);
  return ar_new_number(S, width);
}


ar_Value* ar_graphics_Font_getHeight(ar_State* S, ar_Value* args, ar_Value* env)
{
  graphics_Font* font = (graphics_Font*)ar_eval_udata(S, ar_car(args), env);

  int height = graphics_Font_getHeight(font);

  return ar_new_number(S, height);
}


ar_Value* ar_graphics_Font_getDescent(ar_State* S, ar_Value* args, ar_Value* env)
{
  graphics_Font* font = (graphics_Font*)ar_eval_udata(S, ar_car(args), env);
  return ar_new_number(S, graphics_Font_getDescent(font));
}


ar_Value* ar_graphics_Font_getAscent(ar_State* S, ar_Value* args, ar_Value* env)
{
  graphics_Font* font = (graphics_Font*)ar_eval_udata(S, ar_car(args), env);
  return ar_new_number(S, graphics_Font_getAscent(font));
}


ar_Value* ar_graphics_Font_getBaseline(ar_State* S, ar_Value* args, ar_Value* env)
{
  graphics_Font* font = (graphics_Font*)ar_eval_udata(S, ar_car(args), env);
  return ar_new_number(S, graphics_Font_getBaseline(font));
}


ar_Value* ar_graphics_font_print(ar_State* S, ar_Value* args, ar_Value* env)
{
  char argc = 0;
  graphics_Font* font;
  ar_Value* has_car_as_font = ar_eval(S, ar_car(args), env);
  if (ar_type(has_car_as_font) == AR_TUDATA)
    {
      font = (graphics_Font*) has_car_as_font->u.udata.ptr;
      argc++;
    }
  else
    font = &moduleData.defaultFont;

  char const* text = ar_eval_string(S, ar_nth(args, argc++), env);
  if(!text)
    text = "";

  int x = ar_eval_number(S, ar_nth(args, argc++), env);
  int y = ar_eval_number(S, ar_nth(args, argc++), env);
  float r = ar_opt_number(S, ar_nth(args, argc++), env, 0);
  float sx = ar_opt_number(S, ar_nth(args, argc++), env, 1);
  float sy = ar_opt_number(S, ar_nth(args, argc++), env, 1);
  float ox = ar_opt_number(S, ar_nth(args, argc++), env, 0);
  float oy = ar_opt_number(S, ar_nth(args, argc++), env, 0);
  float kx = ar_opt_number(S, ar_nth(args, argc++), env, 0);
  float ky = ar_opt_number(S, ar_nth(args, argc++), env, 0);
  graphics_Font_render(font, text, x, y, r, sx, sy, ox, oy, kx, ky);
  return NULL;
}

ar_Value* ar_graphics_Font_free(ar_State* S, ar_Value* args, ar_Value* env)
{
  graphics_Font* font = (graphics_Font*) ar_eval_udata(S, ar_car(args), env);
  graphics_Font_free(font);
  free(font);
  return NULL;
}


void ar_graphics_font_register(ar_State* S)
{
  struct { const char* name; ar_Prim fn; } prims[] =
  {
  {"love:graphics-newFont", 				        ar_graphics_newFont},
  {"love:graphics-font-free",           	                ar_graphics_Font_free},
  {"love:graphics-setFont", 					ar_graphics_setFont},
  {"love:graphics-font-getBaseline", 				ar_graphics_Font_getBaseline},
  {"love:graphics-font-getWidth", 				ar_graphics_Font_getWidth},
  {"love:graphics-font-getHeight", 				ar_graphics_Font_getHeight},
  {"love:graphics-font-getDescent", 				ar_graphics_Font_getDescent},
  {"love:graphics-font-getAscent", 				ar_graphics_Font_getAscent},
  {"love:graphics-print", 				        ar_graphics_font_print},
  {NULL, NULL}
};
  for (int i = 0; prims[i].name; i++)
    ar_bind_global(S, prims[i].name, ar_new_prim(S, prims[i].fn));
}




