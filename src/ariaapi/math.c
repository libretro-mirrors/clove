/*
#   clove
#
#   Copyright (C) 2018 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/

#include <time.h>

#include "math.h"

#include "../3rdparty/noise/simplexnoise.h"

#include "../math/minmax.h"
#include "../math/random.h"
#include "../tools/utils.h"

ar_Value* ar_math_random(ar_State* S, ar_Value* args, ar_Value* env)
{
  float rmin = 0.0f;
  float rmax = 1.0f;
  int top = ar_length(args);

  switch (top)
    {
    case 0:
      rmin = 0.0f;
      rmax = 1.0f;
      break;
    case 1:
      rmin = 0.0f;
      rmax = ar_eval_number(S, ar_car(args), env);
      break;
    case 2:
      rmin = ar_eval_number(S, ar_car(args), env);
      rmax = ar_eval_number(S, ar_nth(args, 1), env);
      break;
    }

  return ar_new_number(S, math_random(rmin, rmax));
}


ar_Value* ar_math_random_setSeed(ar_State* S, ar_Value* args, ar_Value* env)
{
  double value = ar_opt_number(S, ar_car(args), env, 0);
  math_random_setSeed(value == 0 ? time(NULL) : value);
  return NULL;
}


ar_Value* ar_math_max(ar_State* S, ar_Value* args, ar_Value* env)
{
  double x = ar_eval_number(S, ar_car(args), env);
  double y = ar_eval_number(S, ar_nth(args, 1), env);
  double result = max(x, y);
  return ar_new_number(S, result);
}


ar_Value* ar_math_min(ar_State* S, ar_Value* args, ar_Value* env)
{
  double x = ar_eval_number(S, ar_car(args), env);
  double y = ar_eval_number(S, ar_nth(args, 1), env);
  double result = min(x, y);
  return ar_new_number(S, result);
}


ar_Value* ar_math_clamp(ar_State* S, ar_Value* args, ar_Value* env)
{
  double a = ar_eval_number(S, ar_car(args), env);
  double x = ar_eval_number(S, ar_nth(args, 1), env);
  double y = ar_eval_number(S, ar_nth(args, 2), env);
  double result = clamp(a, x, y);
  return ar_new_number(S, result);
}


ar_Value* ar_math_noise(ar_State* S, ar_Value* args, ar_Value* env)
{
  float a[4];
  float r;

  int top = min(ar_length(args), 4);

  for (int i = 0; i < top; ++i)
    a[i] = ar_eval_number(S, ar_nth(args, i), env);

  switch (top)
    {
    case 1:
      r = simplexnoise_noise1(a[0]);
      break;
    case 2:
      r = simplexnoise_noise2(a[0], a[1]);
      break;
    case 3:
      r = simplexnoise_noise3(a[0], a[1], a[2]);
      break;
    case 4:
      r = simplexnoise_noise4(a[0], a[1], a[2], a[3]);
      break;

    default:
      clove_error("%s\n","Error: Need at least one dimension");
      return NULL;
    }

  return ar_new_number(S, r);
}


void ar_math_register(ar_State* S)
{
  struct { const char* name; ar_Prim fn; } prims[] =
  {
  {"love:math-noise", 					ar_math_noise},
  {"love:math-random", 					ar_math_random},
  {"love:math-setRandomSeed", 			ar_math_random_setSeed},
  {"love:math-max", 						ar_math_max},
  {"love:math-min", 						ar_math_min},
  {"love:math-clamp", 					ar_math_clamp},
  {NULL, NULL}
};
  for (int i = 0; prims[i].name; i++)
    ar_bind_global(S, prims[i].name, ar_new_prim(S, prims[i].fn));
}


