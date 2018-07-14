/*
#   clove
#
#   Copyright (C) 2018 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
 */

#include "aria_mainactivity.h"

#include "graphics/geometry.h"

#include "3rdparty/aria/aria.h"
#include "ariaapi/graphics_window.h"
#include "ariaapi/graphics.h"
#include "ariaapi/graphics_image.h"
#include "ariaapi/graphics_mesh.h"
#include "ariaapi/input.h"
#include "ariaapi/time.h"
#include "ariaapi/audio.h"
#include "ariaapi/graphics_font.h"
#include "ariaapi/graphics_bitmapfont.h"
#include "ariaapi/math.h"
#include "ariaapi/system.h"
#include "ariaapi/graphics_geometry.h"
#include "ariaapi/filesystem.h"
#include "ariaapi/graphics_quad.h"
#include "ariaapi/graphics_spritebatch.h"
#include "ariaapi/event.h"

static void quit_function(ar_State* S)
{
  ar_call_global_s(S, "love-quit", NULL);
}

static void aria_main_loop(ar_State* S)
{
  timer_step();
  matrixstack_origin();
  ar_input_update(S);

  ar_call_global_s(S, "love-update", ar_new_number(S, timer_getDelta()));

  graphics_clear();
  ar_call_global_s(S, "love-draw", NULL);
  graphics_swap();

  audio_updateStreams();
}

void aria_main_activity_load(int argc, char* argv[])
{
  ar_running = 1;

  keyboard_init();
  joystick_init();
  timer_init();
  filesystem_init(argv[0], 0);
  audio_init(0);
  filesystem_setIdentity(".");

  graphics_init(800, 600, false, true, true);

  ar_State* S = ar_new_state(NULL, NULL);
  if (!S)
    clove_error("out of memory\n");

  ar_graphics_window_register(S);
  ar_graphics_register(S);
  ar_graphics_font_register(S);
  ar_graphics_bitmapfont_register(S);
  ar_graphics_quad_register(S);
  ar_graphics_image_register(S);
  ar_input_register(S);
  ar_time_register(S);
  ar_audio_register(S);
  ar_math_register(S);
  ar_filesystem_register(S);
  ar_system_register(S);
  ar_geometry_register(S);
  ar_graphics_mesh_register(S);
  ar_graphics_SpriteBatch_register(S);
  ar_event_register(S);

  /* Load and do file from argv[1] */
  if (!ar_do_file(S, argv[1]))
    return;

  ar_call_global_s(S, "love-load", NULL);

#ifdef CLOVE_WEB
  //TODO find a way to quit(love.event.quit) love on web?
  emscripten_set_main_loop(main_loop, 60, 1);
#else
  while (ar_running == 1)
    aria_main_loop(S);
#endif

  quit_function(S);

  joystick_close();
  graphics_destroyWindow();
  filesystem_free();
  audio_close();
  graphics_geometry_free();

  ar_close_state(S);
}
