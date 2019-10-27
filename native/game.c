/*
#   clove
#
#   Copyright (C) 2017-2018 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/
#include "game.h"

/*#include <stdbool.h>
#include <stdio.h>

#include "../src/include/graphics.h"
#include "../src/include/geometry.h"
#include "../src/include/matrixstack.h"
#include "../src/include/keyboard.h"
#include "../src/include/ui.h"
#include "../src/luaapi/event.h"
*/
/*static mu_Container *container0 = NULL;
static mu_Container *container1 = NULL;
static mu_Container *container2 = NULL;
*/
void game_load(void) {
    /*container0 = ui_init_window(100, 100, 126, 126, 0);
    container1 = ui_init_window(400, 100, 126, 126, 0);
    container2 = ui_init_window(100, 100, 180, 180, MU_OPT_POPUP);*/
}

void game_update(float delta) {
    //  if (keyboard_ispressed(SDLK_d))
    //    {
    //    }
    //  if (keyboard_ispressed(SDLK_a))
    //    {
    //    }
    /*
    ui_begin();
    static int open = 0;
    if (ui_begin_window("Window 1", container0)) {
        if (ui_header(&open, "show more")) {
            if (ui_begin_tree(&open, "tree")) {
                ui_button("V", MU_OPT_ALIGNLEFT);
                ui_button("L", MU_OPT_ALIGNLEFT);
                ui_button("A", MU_OPT_ALIGNLEFT);
                ui_button("D", MU_OPT_ALIGNLEFT);
                ui_label("Vlad", MU_OPT_ALIGNLEFT);

                ui_end_tree();
            }
        }

        ui_layout_row(3, (int[]) {30, -50, 100}, 0);
        ui_button("X", MU_OPT_ALIGNLEFT);
        static char buf[MU_MAX_TXT_SIZE];
        if (ui_textbox(buf)) {
            mu_set_focus(ui_get_context(), ui_get_context()->last_id);
        }
        ui_slider(0, 0, 300, 0);
        ui_layout_set_next(200, 200, 140, 100, 1);
        ui_button("Submit", MU_OPT_ALIGNLEFT);
        ui_end_window();
    }
    if (ui_begin_window("Window 2", container1)) {
        static mu_Container panel;
        mu_layout_row(ui_get_context(), 1, (int[]) { -1 }, -28);
        mu_begin_panel(ui_get_context(), &panel);
        ui_draw_text("Hello");
        mu_end_panel(ui_get_context());
        ui_end_window();
    }
    ui_end();*/
}

void game_draw(void) {
    //  graphics_setBackgroundColor(.8f, .6f, .5f, 1);
    //  graphics_geometry_rectangle(true, body->p.x, body->p.y, width2, HEIGHT, body->a, 1, 1, 0, 0);
    //  graphics_geometry_rectangle(true, 0, 40, WIDTH, HEIGHT, 0, 1, 1, 0, 0);
    //  //graphics_geometry_rectangle(true, ground->body->p.x, ground->body->p.y, 800, 32, 0, 1, 1, 0, 0);
}

void game_quit(void) {
}

