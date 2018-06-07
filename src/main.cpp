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

#include <stdio.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

extern "C" {
	#include "lua_mainactivity.h"
	#include "aria_mainactivity.h"
}

int main(int argc, char* argv[]) {

	if (argv[1] != NULL)
		aria_main_activity_load(argc, argv);
	else
		lua_main_activity_load(argc, argv);
    return EXIT_SUCCESS;
}
