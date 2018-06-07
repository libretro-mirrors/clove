/*
#   clove
#
#   Copyright (C) 2018 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/
#include "time.h"

#include "../timer/timer.h"


ar_Value* ar_time_step(ar_State* S, ar_Value* args, ar_Value* env)
{
	timer_step();
	return NULL;
}


ar_Value* ar_time_getFPS(ar_State* S, ar_Value* args, ar_Value* env)
{
	return ar_new_number(S, (int)(timer_getFPS()+0.5f));
}


ar_Value* ar_time_getTime(ar_State* S, ar_Value* args, ar_Value* env)
{
	return ar_new_number(S, timer_getTime());
}


ar_Value* ar_time_getDelta(ar_State* S, ar_Value* args, ar_Value* env)
{
	return ar_new_number(S, timer_getDelta());
}


ar_Value* ar_time_getAverageDelta(ar_State* S, ar_Value* args, ar_Value* env)
{
	return ar_new_number(S, timer_getAverageDelta());
}


ar_Value* ar_time_sleep(ar_State* S, ar_Value* args, ar_Value* env)
{
	double seconds = ar_eval_number(S, ar_car(args), env);
	timer_sleep(seconds);
	return NULL;
}


void ar_time_register(ar_State* S)
{
	struct { const char* name; ar_Prim fn; } prims[] =
	{
		{"love:timer-getFPS", 			ar_time_getFPS},
		{"love:timer-getTime", 			ar_time_getTime},
		{"love:timer-sleep", 				ar_time_sleep},
		{"love:timer-getDelta", 			ar_time_getDelta},
		{"love:timer-getAverageDelta", 	ar_time_getAverageDelta},
		{"love:timer-update", 				ar_time_step},
		{NULL, NULL}
	};
	for (int i = 0; prims[i].name; i++)
		ar_bind_global(S, prims[i].name, ar_new_prim(S, prims[i].fn));
}


