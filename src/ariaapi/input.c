/*
#   clove
#
#   Copyright (C) 2018 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/
#include "input.h"
#include "event.h"

#include "../graphics/graphics.h"

static struct {
  int key_down;
  int key_up;
  int mouse_x;
  int mouse_y;
} moduleData;

void ar_input_update(ar_State* S)
{
  while(SDL_PollEvent(&aria_input_event)) {
      if (aria_input_event.type == SDL_WINDOWEVENT) {
          switch (aria_input_event.window.event) {
            case SDL_WINDOWEVENT_ENTER:
              graphics_setMouseFocus(1);
              break;
            case SDL_WINDOWEVENT_LEAVE:
              graphics_setMouseFocus(0);
              break;
            case SDL_WINDOWEVENT_FOCUS_LOST:
              graphics_setFocus(0);
              break;
            case SDL_WINDOWEVENT_FOCUS_GAINED:
              graphics_setFocus(1);
              break;
            default:
              break;
            }
        }
      switch(aria_input_event.wheel.type) {
        case SDL_MOUSEWHEEL:
          ar_call_global_s(S, "love-wheelmoved", ar_new_number(S,aria_input_event.wheel.y));
          break;
        default:
          break;
        }
      switch (aria_input_event.type) {
        case SDL_QUIT:
          ar_running = 0;
          break;
        case SDL_KEYDOWN:
          moduleData.key_up = -1;
          moduleData.key_down = aria_input_event.key.keysym.sym;
          break;
        case SDL_KEYUP:
          moduleData.key_down = -1;
          moduleData.key_up = aria_input_event.key.keysym.sym;
          break;
        case SDL_TEXTINPUT:
          ar_call_global_s(S, "love-textinput", ar_new_string(S, aria_input_event.text.text));
          break;
        case SDL_MOUSEBUTTONDOWN:
          ar_call_global_s(S, "love-mousepressed", ar_new_list(S, 3, ar_new_number(S, aria_input_event.button.x),
                                                               ar_new_number(S, aria_input_event.button.y),
                                                               ar_new_number(S, aria_input_event.button.button)));
          break;
        case SDL_MOUSEBUTTONUP:
          ar_call_global_s(S, "love-mousereleased", ar_new_list(S, 3, ar_new_number(S, aria_input_event.button.x),
                                                                ar_new_number(S, aria_input_event.button.y),
                                                                ar_new_number(S, aria_input_event.button.button)));
          break;
        case SDL_MOUSEMOTION:
          moduleData.mouse_x = aria_input_event.motion.x;
          moduleData.mouse_y = aria_input_event.motion.y;
          break;
        }
    }
}

ar_Value* ar_keyboard_keypressed(ar_State* S, ar_Value* args, ar_Value* env)
{
  return ar_new_number(S, moduleData.key_down);
}


ar_Value* ar_keyboard_keyreleased(ar_State* S, ar_Value* args, ar_Value* env)
{
  return ar_new_number(S, moduleData.key_up);
}

ar_Value* ar_mouse_x(ar_State* S, ar_Value* args, ar_Value* env)
{
  return ar_new_number(S, moduleData.mouse_x);
}


ar_Value* ar_mouse_y(ar_State* S, ar_Value* args, ar_Value* env)
{
  return ar_new_number(S, moduleData.mouse_y);
}


void ar_input_register(ar_State* S)
{
  moduleData.key_down = -1;
  moduleData.key_up = -1;
  moduleData.mouse_x= -1;
  moduleData.mouse_y = -1;
  struct { const char* name; ar_Prim fn; } prims[] =
  {
  {"love:mouse-x", ar_mouse_x},
  {"love:mouse-y", ar_mouse_y},
  {"love:keyboard-pressed", ar_keyboard_keypressed},
  {"love:keyboard-released", ar_keyboard_keyreleased},
  {NULL, NULL}
};
  for (int i = 0; prims[i].name; i++)
    ar_bind_global(S, prims[i].name, ar_new_prim(S, prims[i].fn));
}


