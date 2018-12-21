/*
#   clove
#
#   Copyright (C) 2016-2018 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/
#pragma once

#include <tgmath.h>


#define LOVE_PI 3.141592653589793238462643383279502884L


inline void normalizeInPlace(float *x, float *y) {
  float a = *x;
  float b = *y;
  float li = sqrt(a*a+b*b);
  if(li == 0.0f) {
    return;
  }

  li = 1.0f / li;
  *x *= li;
  *y *= li;
}
