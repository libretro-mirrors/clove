/*
#   clove
#
#   Copyright (C) 2017-2018 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/
#include "game.h"

#include <stdbool.h>
#include <stdio.h>

#include "../src/include/graphics.h"
#include "../src/include/geometry.h"
#include "../src/include/matrixstack.h"
#include "../src/include/keyboard.h"
#include "../src/include/ui.h"
#include "../src/luaapi/event.h"

static mu_Container *container0 = NULL;

void game_load(void) {
    ui_init();
    container0 = ui_init_window(100, 100, 126, 126);
}

void game_update(float delta) {
//  if (keyboard_ispressed(SDLK_d))
//    {
//    }
//  if (keyboard_ispressed(SDLK_a))
//    {
//    }
    ui_begin();
    if (ui_begin_window("Test window", container0)) {
        ui_end_window();
    }
    ui_end();
}

void game_draw(void) {
//  graphics_setBackgroundColor(.8f, .6f, .5f, 1);
//  graphics_geometry_rectangle(true, body->p.x, body->p.y, width2, HEIGHT, body->a, 1, 1, 0, 0);
//  graphics_geometry_rectangle(true, 0, 40, WIDTH, HEIGHT, 0, 1, 1, 0, 0);
//  //graphics_geometry_rectangle(true, ground->body->p.x, ground->body->p.y, 800, 32, 0, 1, 1, 0, 0);
    ui_draw();
}

void game_quit(void) {
    ui_deinit_window(container0);
    ui_deinit();
}

