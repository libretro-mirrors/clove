/*
#   clove
#
#   Copyright (C) 2018 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/
#include "graphics_window.h"

#include "../graphics/graphics.h"


ar_Value* ar_graphics_window_setMode(ar_State* S, ar_Value* args, ar_Value* env)
{
  float w = ar_eval_number(S, ar_car(args), env);
  float h = ar_eval_number(S, ar_nth(args, 1), env);
  bool fullscreen = ar_opt_number(S, ar_nth(args, 2), env, 0);
  bool vsync = ar_opt_number(S, ar_nth(args, 3), env, 0);
  float m_s_x = ar_opt_number(S, ar_nth(args, 4), env, 0);
  float m_s_y = ar_opt_number(S, ar_nth(args, 5), env, 0);
  float ma_s_x = ar_opt_number(S, ar_nth(args, 6), env, 0);
  float ma_s_y = ar_opt_number(S, ar_nth(args, 7), env, 0);
  bool border = ar_opt_number(S, ar_nth(args, 8), env, 1);
  float x = ar_opt_number(S, ar_nth(args, 9), env, -1);
  float y = ar_opt_number(S, ar_nth(args, 10), env, -1);
  graphics_setMode(w, h, fullscreen, vsync, m_s_x, m_s_y, ma_s_x, ma_s_y, border, x, y);
  return NULL;
}


ar_Value* ar_graphics_window_getWidth(ar_State* S, ar_Value* args, ar_Value* env)
{
  return ar_new_number(S, graphics_getWidth());
}


ar_Value* ar_graphics_window_getHeight(ar_State* S, ar_Value* args, ar_Value* env)
{
  return ar_new_number(S, graphics_getHeight());
}


ar_Value* ar_graphics_window_setTitle(ar_State* S, ar_Value* args, ar_Value* env)
{
  graphics_setTitle(ar_eval_string(S, ar_car(args), env));
  return NULL;
}


ar_Value* ar_graphics_window_getDesktopDimension(ar_State* S, ar_Value* args, ar_Value* env)
{
  return ar_new_pair(S, ar_new_number(S, graphics_getDesktopDimension()[0]),
      ar_new_number(S, graphics_getDesktopDimension()[1]));
}


ar_Value* ar_graphics_window_getDisplayCount(ar_State* S, ar_Value* args, ar_Value* env)
{
  return ar_new_number(S, graphics_getDisplayCount());
}


ar_Value* ar_graphics_window_getIcon(ar_State* S, ar_Value* args, ar_Value* env)
{
  return ar_new_string(S, image_ImageData_getPath(graphics_getIcon()));
}


ar_Value* ar_graphics_window_setIcon(ar_State* S, ar_Value* args, ar_Value* env)
{
  image_ImageData* img = (image_ImageData*) ar_eval_udata(S, ar_car(args), env);
  graphics_setIcon(img);
  return NULL;
}


ar_Value* ar_graphics_window_getDisplayName(ar_State* S, ar_Value* args, ar_Value* env)
{
  return ar_new_string(S, graphics_getDisplayName(ar_eval_number(S, ar_car(args), env)));
}


ar_Value* ar_graphics_window_setFullscreen(ar_State* S, ar_Value* args, ar_Value* env)
{
  graphics_setFullscreen(ar_eval_boolean(S, ar_car(args), env),
                         ar_eval_string(S, ar_nth(args, 1), env));
  return NULL;
}


ar_Value* ar_graphics_window_isCreated(ar_State* S, ar_Value* args, ar_Value* env)
{
  if (graphics_isCreated())
    return S->t;
  return NULL;
}


ar_Value* ar_graphics_window_getTitle(ar_State* S, ar_Value* args, ar_Value* env)
{
  return ar_new_string(S, graphics_getTitle());
}


ar_Value* ar_graphics_window_setPosition(ar_State* S, ar_Value* args, ar_Value* env)
{
  graphics_setPosition(
        ar_eval_number(S, ar_car(args), env),
        ar_eval_number(S, ar_nth(args, 1), env)
        );
  return NULL;
}


ar_Value* ar_graphics_window_hasFocus(ar_State* S, ar_Value* args, ar_Value* env)
{
  if (graphics_hasFocus())
    return S->t;
  return NULL;
}


ar_Value* ar_graphics_window_hasMouseFocus(ar_State* S, ar_Value* args, ar_Value* env)
{
  if (graphics_hasMouseFocus())
    return S->t;
  return NULL;
}


ar_Value* ar_graphics_window_getDimensions(ar_State* S, ar_Value* args, ar_Value* env)
{
  return ar_new_pair(S, ar_new_number(S, graphics_getWidth()),
                     ar_new_number(S, graphics_getHeight()));
}


void ar_graphics_window_register(ar_State* S)
{
  struct { const char* name; ar_Prim fn; } prims[] =
  {
  {"love:window-setMode", 				ar_graphics_window_setMode},
  {"love:window-setFullscreen", 			ar_graphics_window_setFullscreen},
  {"love:window-getDimensions", 			ar_graphics_window_getDimensions},
  {"love:window-getDisplayCount", 		        ar_graphics_window_getDisplayCount},
  {"love:window-getDesktopDimension", 	                ar_graphics_window_getDesktopDimension},
  {"love:window-setIcon", 				ar_graphics_window_setIcon},
  {"love:window-getIcon", 				ar_graphics_window_getIcon},
  {"love:window-getDisplayName", 			ar_graphics_window_getDisplayName},
  {"love:window-isCreated", 				ar_graphics_window_isCreated},
  {"love:window-setTitle", 				ar_graphics_window_setTitle},
  {"love:window-getTitle", 				ar_graphics_window_getTitle},
  {"love:window-setPosition", 			        ar_graphics_window_setPosition},
  {"love:window-getWidth", 				ar_graphics_window_getWidth},
  {"love:window-getHeight", 				ar_graphics_window_getHeight},
  {"love:window-hasFocus", 				ar_graphics_window_hasFocus},
  {"love:window-hasMouseFocus", 			ar_graphics_window_hasMouseFocus},
  {NULL, NULL}
};

  for (int i = 0; prims[i].name; i++)
    ar_bind_global(S, prims[i].name, ar_new_prim(S, prims[i].fn));
}

