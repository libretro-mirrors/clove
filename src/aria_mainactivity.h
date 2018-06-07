/*
#   clove
#
#   Copyright (C) 2018 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/

#ifndef _ARIA_MAIN_ACTIVITY_H_
#define _ARIA_MAIN_ACTIVITY_H_

#include "tools/utils.h"
#include "graphics/graphics.h"
#include "graphics/matrixstack.h"
#include "filesystem/filesystem.h"
#include "audio/audio.h"
#include "audio/streamsource.h"
#include "timer/timer.h"

#include "love.h"
#include "joystick.h"
#include "keyboard.h"
#include "mouse.h"

void aria_main_activity_load(int argc, char* argv[]);

#endif

