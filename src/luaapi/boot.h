/*
#   clove
#
#   Copyright (C) 2016-2017 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/
#pragma once

#include "../3rdparty/lua/lualib.h"
#include "../love_config.h"

int l_boot(lua_State* state, love_Config *config);
int l_no_game(lua_State* state, love_Config *config);
