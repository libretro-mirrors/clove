/*
#   clove
#
#   Copyright (C) 2015-2018 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/

#ifdef EMSCRIPTEN
# include <emscripten.h>
#endif

#include <stdlib.h>
#include <stdio.h>

#include "include/utils.h"

extern "C" {
	#include "include/lua_mainactivity.h"
}

int main(int argc, char* argv[]) {
  #ifdef USE_LUA
	lua_main_activity_load(argc, argv);
  #endif
  return 0;
}
