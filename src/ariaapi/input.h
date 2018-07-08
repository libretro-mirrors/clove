/*
#   clove
#
#   Copyright (C) 2018 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/
#ifndef _AR_INPUT_H_
#define _AR_INPUT_H_

#include "../3rdparty/aria/aria.h"
#include "../3rdparty/SDL2/include/SDL_keyboard.h"
#include "../3rdparty/SDL2/include/SDL.h"

void ar_input_register(ar_State* S);

void ar_input_update(void);

SDL_Event aria_input_event;

#endif
