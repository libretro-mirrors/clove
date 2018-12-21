/*
#   clove
#
#   Copyright (C) 2018 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/


// NOTE: 't' must be form 0 .. 1
inline float lerp(float a, float b, float t) {
  return a + (b-a) * t;
}
