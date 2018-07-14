/*
#   clove
#
#   Copyright (C) 2018 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/
#include "graphics_bitmapfont.h"

#include "../graphics/bitmap_font.h"
#include "../graphics/matrixstack.h"
#include "../graphics/shader.h"
#include "../graphics/graphics.h"

static struct
{
  graphics_BitmapFont* currentFont;
} moduleData;


ar_Value* ar_graphics_newBitmapFont(ar_State* S, ar_Value* args, ar_Value* env)
{
  char const* filename = ar_eval_string(S, ar_car(args), env);
  char const* glyphs = ar_eval_string(S, ar_nth(args, 1), env);
  int max_size = ar_opt_number(S, ar_nth(args, 2), env, 128);
  float glyph_width = ar_opt_number(S, ar_nth(args, 3), env, 0);
  float glyph_height = ar_opt_number(S, ar_nth(args, 4), env, 0);

  graphics_BitmapFont* bm_font = malloc(sizeof(graphics_BitmapFont));
  graphics_BitmapFont_new(bm_font, filename, max_size, glyphs, glyph_width, glyph_height);

  moduleData.currentFont = bm_font;

  return ar_new_udata(S, bm_font);
}


static ar_Value* ar_graphics_BitmapFont_setOffsetX(ar_State* S, ar_Value* args, ar_Value* env)
{
  graphics_BitmapFont* font = (graphics_BitmapFont*)
      ar_eval_udata(S, ar_car(args), env);

  float offset = ar_eval_number(S, ar_nth(args, 1), env);
  char const* glyph = ar_eval_string(S, ar_nth(args, 2), env);
  graphics_BitmapFont_setGlyphOffsetX(font, offset, glyph);
  return NULL;
}


static ar_Value* ar_graphics_BitmapFont_setOffsetY(ar_State* S, ar_Value* args, ar_Value* env)
{
  graphics_BitmapFont* font = (graphics_BitmapFont*)
      ar_eval_udata(S, ar_car(args), env);

  float offset = ar_eval_number(S, ar_nth(args, 1), env);
  char const* glyph = ar_eval_string(S, ar_nth(args, 2), env);
  graphics_BitmapFont_setGlyphOffsetY(font, offset, glyph);
  return NULL;
}


static ar_Value* ar_graphics_BitmapFont_setFont(ar_State* S, ar_Value* args, ar_Value* env)
{
  graphics_BitmapFont* font = (graphics_BitmapFont*)
      ar_eval_udata(S, ar_car(args), env);
  moduleData.currentFont = font;
  return NULL;
}


static ar_Value* ar_graphics_BitmapFont_printb(ar_State* S, ar_Value* args, ar_Value* env)
{
  if (!moduleData.currentFont)
    return NULL;

  const char* const text = ar_eval_string(S, ar_car(args), env);
  int x = ar_eval_number(S, ar_nth(args, 1), env);
  int y = ar_eval_number(S, ar_nth(args, 2), env);
  int r = ar_opt_number(S, ar_nth(args, 3), env, 0);
  int sx = ar_opt_number(S, ar_nth(args, 4), env, 1);
  int sy = ar_opt_number(S, ar_nth(args, 5), env, 1);
  int ox = ar_opt_number(S, ar_nth(args, 6), env, 0);
  int oy = ar_opt_number(S, ar_nth(args, 7), env, 0);
  int kx = ar_opt_number(S, ar_nth(args, 8), env, 0);
  int ky = ar_opt_number(S, ar_nth(args, 9), env, 0);
  graphics_BitmapFont_render(moduleData.currentFont, text, x, y, r,
                             sx, sy, ox, oy, kx, ky);
  return S->t;
}


static ar_Value* ar_graphics_BitmapFont_free(ar_State* S, ar_Value* args, ar_Value* env)
{
  graphics_BitmapFont* font = (graphics_BitmapFont*)
      ar_eval_udata(S, ar_car(args), env);
  graphics_BitmapFont_free(font);
  free(font);
  return NULL;
}


void ar_graphics_bitmapfont_register(ar_State* S)
{
  struct { const char* name; ar_Prim fn; } prims[] =
  {
  {"love:graphics-newImageFont", ar_graphics_newBitmapFont},
  {"love:graphics-bitmapfont-free", ar_graphics_BitmapFont_free},
  {"love:graphics-bitmapfont-setFont",ar_graphics_BitmapFont_setFont},
  {"love:graphics-bitmapfont-setOffsetX", ar_graphics_BitmapFont_setOffsetX},
  {"love:graphics-bitmapfont-setOffsetY", ar_graphics_BitmapFont_setOffsetY},
  {"love:graphics-printb", ar_graphics_BitmapFont_printb},
  {NULL, NULL}
};
  for (int i = 0; prims[i].name; i++)
    ar_bind_global(S, prims[i].name, ar_new_prim(S, prims[i].fn));
}

