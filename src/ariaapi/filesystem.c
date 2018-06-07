/*
#   clove
#
#   Copyright (C) 2018 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/
#include <stdlib.h>
#include <stdio.h>

#include "../filesystem/filesystem.h"

#include "filesystem.h"


ar_Value* ar_filesystem_read(ar_State* S, ar_Value* args, ar_Value* env)
{
	char const* filename = ar_eval_string(S, ar_car(args), env);
	char* data = NULL;
	int len = filesystem_read(filename, &data);
	if (len < 0)
	{
		printf("Could not read file: %s", filename);
		return NULL;
	}
	return ar_new_pair(S, ar_new_string(S, data), ar_new_number(S, len));
}


ar_Value* ar_filesystem_getSaveDirectory(ar_State* S, ar_Value* args, ar_Value* env)
{
	const char* projName = ar_eval_string(S, ar_car(args), env);
	if (strcmp(projName, "") == 0)
		projName = "myGame";
	const char* company = ar_eval_string(S, ar_nth(args, 1), env);
	if (strcmp(company, "") == 0)
		company = "CLove";
	return ar_new_string(S, filesystem_getSaveDirectory(company, projName));
}


ar_Value* ar_filesystem_exists(ar_State* S, ar_Value* args, ar_Value* env)
{
	char const* filename = ar_eval_string(S, ar_car(args), env);
	if (filesystem_exists(filename))
		return S->t;
	return NULL;
}


ar_Value* ar_filesystem_write(ar_State* S, ar_Value* args, ar_Value* env)
{
	char const* filename = ar_eval_string(S, ar_car(args), env);
	char const* data = ar_eval_string(S, ar_nth(args, 1), env);
	filesystem_write(filename, data);
	return NULL;
}


ar_Value* ar_filesystem_append(ar_State* S, ar_Value* args, ar_Value* env)
{
	char const* filename = ar_eval_string(S, ar_car(args), env);
	char const* data = ar_eval_string(S, ar_nth(args, 1), env);
	filesystem_append(filename, data);
	return NULL;
}


ar_Value* ar_filesystem_getSource(ar_State* S, ar_Value* args, ar_Value* env)
{
	return ar_new_string(S, filesystem_getSource());
}


ar_Value* ar_filesystem_getCurrentDirectory(ar_State* S, ar_Value* args, ar_Value* env)
{
	return ar_new_string(S, filesystem_getCurrentDirectory());
}


ar_Value* ar_filesystem_remove(ar_State* S, ar_Value* args, ar_Value* env)
{
	char const* filename = ar_eval_string(S, ar_car(args), env);
	filesystem_remove(filename);
	return NULL;
}


ar_Value* ar_filesystem_contain(ar_State* S, ar_Value* args, ar_Value* env)
{
	char const* a = ar_eval_string(S, ar_car(args), env);
	char const* b = ar_eval_string(S, ar_nth(args, 1), env);
	if (filesystem_contain(a, b))
		return S->t;
	return NULL;
}


ar_Value* ar_filesystem_equals(ar_State* S, ar_Value* args, ar_Value* env)
{
	char const* a = ar_eval_string(S, ar_car(args), env);
	char const* b = ar_eval_string(S, ar_nth(args, 1), env);
	int l = ar_eval_number(S, ar_nth(args, 2), env);

	if (filesystem_equals(a, b, l))
		return S->t;
	return NULL;
}


ar_Value* ar_filesystem_isFile(ar_State* S, ar_Value* args, ar_Value* env)
{
	char const* file = ar_eval_string(S, ar_car(args), env);
	char const* string_mode = ar_eval_string(S, ar_nth(args, 1), env);
	if (strcmp(string_mode, "") == 0)
		string_mode = "e";
	int mode = 0;

    if (strncmp(string_mode,"e", 1) == 0)
        mode = 0;
    else if (strncmp(string_mode,"w", 1) == 0)
        mode = 2;
    else if (strncmp(string_mode,"r", 1) == 0)
        mode = 4;
    else if (strncmp(string_mode,"wr", 2) == 0)
        mode = 6;

    int isFile = filesystem_isFile(file, mode);

    if (isFile == 0)
		return S->t;

	return NULL;
}


ar_Value* ar_filesystem_setSource(ar_State* S, ar_Value* args, ar_Value* env)
{
	char const* source = ar_eval_string(S, ar_car(args), env);
	char const* dir = ar_eval_string(S, ar_nth(args, 1), env);
	filesystem_setSource(source, dir);
	return NULL;
}


ar_Value* ar_filesystem_getUsrDir(ar_State* S, ar_Value* args, ar_Value* env)
{
	return ar_new_string(S, filesystem_getUsrDir());
}


ar_Value* ar_filesystem_mkDir(ar_State* S, ar_Value* args, ar_Value* env)
{
	char const* dir = ar_eval_string(S, ar_car(args), env);
	filesystem_mkDir(dir);
	return NULL;
}


ar_Value* ar_filesystem_isDir(ar_State* S, ar_Value* args, ar_Value* env)
{
	char const* dir = ar_eval_string(S, ar_car(args), env);
	if (filesystem_isDir(dir))
		return S->t;
	return NULL;
}


ar_Value* ar_filesystem_setIdentity(ar_State* S, ar_Value* args, ar_Value* env)
{
	char const* path = ar_eval_string(S, ar_car(args), env);
	if (filesystem_setIdentity(path))
		return S->t;
	return NULL;
}


void ar_filesystem_register(ar_State* S)
{
	struct { const char* name; ar_Prim fn; } prims[] =
	{
		{"love:filesystem-read", ar_filesystem_read},
		{"love:filesystem-getSaveDirectory", ar_filesystem_getSaveDirectory},
		{"love:filesystem-exists", ar_filesystem_exists},
		{"love:filesystem-write", ar_filesystem_write},
		{"love:filesystem-append", ar_filesystem_append},
		{"love:filesystem-getSource", ar_filesystem_getSource},
		{"love:filesystem-getCurrentDirectory", ar_filesystem_getCurrentDirectory},
		{"love:filesystem-remove", ar_filesystem_remove},
		{"love:filesystem-contain", ar_filesystem_contain},
		{"love:filesystem-equals", ar_filesystem_equals},
		{"love:filesystem-isFile", ar_filesystem_isFile},
		{"love:filesystem-setSource", ar_filesystem_setSource},
		{"love:filesystem-getUsrDir", ar_filesystem_getUsrDir},
		{"love:filesystem-mkDir", ar_filesystem_mkDir},
		{"love:filesystem-isDir", ar_filesystem_isDir},
		{"love:filesystem-setIdentity", ar_filesystem_setIdentity},
		{NULL, NULL}
	};

	for (int i = 0; prims[i].name; i++)
		ar_bind_global(S, prims[i].name, ar_new_prim(S, prims[i].fn));
}




