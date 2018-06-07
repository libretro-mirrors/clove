/*
#   clove
#
#   Copyright (C) 2018 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/

#include "system.h"

#include <stdlib.h>
#include <stdio.h>

#include "../system.h"


ar_Value* ar_system_getOS(ar_State* S, ar_Value* args, ar_Value* env)
{
	return	ar_new_string(S, system_getOS());
}


ar_Value* ar_system_getPowerInfo(ar_State* S, ar_Value* args, ar_Value* env)
{
	system_PowerState power = system_getPowerInfo();
	return ar_new_list(S, 3,
			ar_new_string(S, power.state),
			ar_new_number(S, power.seconds),
			ar_new_number(S, power.percent));
}


ar_Value* ar_system_getProcessorCount(ar_State* S, ar_Value* args, ar_Value* env)
{
	return ar_new_number(S, system_getProcessorCount());
}


ar_Value* ar_system_getClipboardText(ar_State* S, ar_Value* args, ar_Value* env)
{
	return ar_new_string(S, system_getClipboardText());
}


ar_Value* ar_system_setClipboardText(ar_State* S, ar_Value* args, ar_Value* env)
{
	const char* text = ar_eval_string(S, ar_car(args), env);
	system_setClipboardText(text);
	return NULL;
}


void ar_system_register(ar_State* S)
{
	struct { const char* name; ar_Prim fn; } prims[] =
	{
		{"love:system-getPowerInfo", 				ar_system_getPowerInfo},
		{"love:system-getOS", 						ar_system_getOS},
		{"love:system-getProcessorCount", 			ar_system_getProcessorCount},
		{"love:system-getClipboardText", 			ar_system_getClipboardText},
		{"love:system-setClipboardText", 			ar_system_setClipboardText},
		{NULL, NULL}
	};
	for (int i = 0; prims[i].name; i++)
		ar_bind_global(S, prims[i].name, ar_new_prim(S, prims[i].fn));
}


