/*
#   clove
#
#   Copyright (C) 2018 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/
#include "input.h"

#include "../3rdparty/SDL2/include/SDL_keyboard.h"
#include "../3rdparty/SDL2/include/SDL.h"

SDL_Event event;


ar_Value* ar_event_quit(ar_State* S, ar_Value* args, ar_Value* env)
{
	if (event.type == SDL_QUIT)
		return S->t;
	return NULL;
}


ar_Value* ar_keyboard_update(ar_State* S, ar_Value* args, ar_Value* env)
{
	SDL_PollEvent(&event);
	return NULL;
}


ar_Value* ar_keyboard_keypressed(ar_State* S, ar_Value* args, ar_Value* env)
{
	if (event.type == SDL_KEYDOWN)
		return ar_new_number(S, event.key.keysym.sym);
	return NULL;
}


ar_Value* ar_keyboard_keyreleased(ar_State* S, ar_Value* args, ar_Value* env)
{
	if (event.type == SDL_KEYUP)
		return ar_new_number(S, event.key.keysym.sym);
	return NULL;
}


ar_Value* ar_mouse_mousepressed(ar_State* S, ar_Value* args, ar_Value* env)
{
	if (event.type == SDL_MOUSEBUTTONDOWN)
	{
		return ar_new_list(S, 3, ar_new_number(S, event.button.x),
				ar_new_number(S, event.button.y),
				ar_new_number(S, event.button.button));
	}
	return NULL;
}


ar_Value* ar_mouse_mousereleased(ar_State* S, ar_Value* args, ar_Value* env)
{
	if (event.type == SDL_MOUSEBUTTONUP)
	{
		return ar_new_list(S, 3, ar_new_number(S, event.button.x),
				ar_new_number(S, event.button.y),
				ar_new_number(S, event.button.button));
	}
	return NULL;
}


ar_Value* ar_mouse_mousewheel(ar_State* S, ar_Value* args, ar_Value* env)
{
	if (event.type == SDL_MOUSEBUTTONUP)
	{
		int _what = event.wheel.y == 1 ? SDL_BUTTON_WHEEL_UP : SDL_BUTTON_WHEEL_DOWN;
		return ar_new_number(S, _what);
	}
	return NULL;
}


ar_Value* ar_mouse_mousemoved(ar_State* S, ar_Value* args, ar_Value* env)
{
	if (event.type == SDL_MOUSEMOTION)
		return ar_new_pair(S, ar_new_number(S, event.motion.x),
				ar_new_number(S, event.motion.y));
	return NULL;
}


void ar_input_register(ar_State* S)
{
	struct { const char* name; ar_Prim fn; } prims[] =
	{
		{"love:event-quit",      ar_event_quit},
		{"love:keyboard-update", ar_keyboard_update},

		{"love:mouse-pressed", ar_mouse_mousepressed},
		{"love:mouse-released", ar_mouse_mousereleased},
		{"love:mouse-wheel", ar_mouse_mousewheel},
		{"love:mouse-moved", ar_mouse_mousemoved},

		{"love:keyboard-keypressed", ar_keyboard_keypressed},
		{"love:keyboard-keyreleased", ar_keyboard_keyreleased},
		{NULL, NULL}
	};
	for (int i = 0; prims[i].name; i++)
		ar_bind_global(S, prims[i].name, ar_new_prim(S, prims[i].fn));
}


