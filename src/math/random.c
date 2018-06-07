/*
#   clove
#
#   Copyright (C) 2016-2017 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/
#include <stdlib.h>
#include "random.h"

float math_random(float min, float max) {
  return (rand() % (int)((max+1) - min)) + min;
}

void math_random_setSeed(double value) {
  srand(value);
}
