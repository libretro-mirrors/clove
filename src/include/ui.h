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

mu_Container* ui_init_window(int x, int y, int w, int h);
void ui_deinit_window(mu_Container* window);
int ui_begin_window(const char *title, mu_Container *window);
void ui_end_window(void);

int ui_button(const char* label);

void ui_begin(void);
void ui_end(void);
void ui_draw(void);

void ui_input_mouse_move(int x, int y);
void ui_input_mouse_down(int btn, int x, int y);
void ui_input_mouse_up(int btn, int x, int y);
void ui_input_text(const char *txt);
void ui_input_scroll(int x, int y);
void ui_input_keydown(int key);
void ui_input_keyup(int key);
