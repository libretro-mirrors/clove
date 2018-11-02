/*
#   clove
#
#   Copyright (C) 2016-2018 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/
#ifndef UTILS_H_
#define UTILS_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define SAFE_FREE(o) {  if (o) { free(o); o = NULL; } }

#define CLOVE_DEBUG(x) { printf("CLove Debug: %s %s %s %s %s \n", x , " at line " __LINE__, " in file  ", __FILE__ ); }

void clove_error(const char* format, ...);


#ifndef USE_NATIVE
    #define USE_NATIVE 100
#endif

#if defined(linux) || defined(__linux) || defined(__linux__)
#	define CLOVE_LINUX 1
#endif

#if defined(__APPLE__)
#	define CLOVE_MACOSX 2
#endif

#if defined(WIN32) || defined(_WIN32)
#	define CLOVE_WINDOWS 3
#endif

#if defined(EMSCRIPTEN) || defined(__EMSCRIPTEN__)
#  define CLOVE_WEB 4
#  define CLOVE_ES 10
#endif

#if defined(__ANDROID__) || defined(ANDROID)
# 	define CLOVE_ANDROID 5
#   define CLOVE_ES 10
#endif

#if defined(CLOVE_MACOSX) || defined(CLOVE_LINUX)
#   define CLOVE_UNIX 20
# 	define CLOVE_GL 11
#endif

#if defined(CLOVE_MACOSX) || defined(CLOVE_LINUX) || defined(CLOVE_WINDOWS)
#   define CLOVE_DESKTOP 20
# 	define CLOVE_GL 11
#endif

char* util_concatenate(const char* first, const char* second);

#endif
