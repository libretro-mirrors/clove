/*
#   clove
#
#   Copyright (C) 2019 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/
#pragma once

#include "../3rdparty/microui/src/microui.h"

void ui_init(void);
void ui_deinit(void);

// window
mu_Container* ui_init_window(int x, int y, int w, int h);
void ui_deinit_window(mu_Container* window);
int ui_begin_window(char* title, mu_Container *window);
void ui_end_window(void);

void ui_begin(void);
void ui_end(void);
void ui_draw(void);
