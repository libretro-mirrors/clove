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

static int keydown = -1;
static int keyup = -1;

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
      switch (aria_input_event.type) {
        case SDL_QUIT:
          ar_running = 0;
          break;
        case SDL_KEYDOWN:
          keyup = -1;
          keydown = aria_input_event.key.keysym.sym;
          break;
        case SDL_KEYUP:
          keydown = -1;
          keyup = aria_input_event.key.keysym.sym;
          break;
      }
    }
}

ar_Value* ar_keyboard_keypressed(ar_State* S, ar_Value* args, ar_Value* env)
{
    return ar_new_number(S, keydown);
}


ar_Value* ar_keyboard_keyreleased(ar_State* S, ar_Value* args, ar_Value* env)
{
    return ar_new_number(S, keyup);
}


ar_Value* ar_mouse_mousepressed(ar_State* S, ar_Value* args, ar_Value* env)
{
  if (aria_input_event.type == SDL_MOUSEBUTTONDOWN)
    {
      return ar_new_list(S, 3, ar_new_number(S, aria_input_event.button.x),
                         ar_new_number(S, aria_input_event.button.y),
                         ar_new_number(S, aria_input_event.button.button));
    }
  return NULL;
}


ar_Value* ar_mouse_mousereleased(ar_State* S, ar_Value* args, ar_Value* env)
{
  if (aria_input_event.type == SDL_MOUSEBUTTONUP)
    {
      return ar_new_list(S, 3, ar_new_number(S, aria_input_event.button.x),
                         ar_new_number(S, aria_input_event.button.y),
                         ar_new_number(S, aria_input_event.button.button));
    }
  return NULL;
}


ar_Value* ar_mouse_mousewheel(ar_State* S, ar_Value* args, ar_Value* env)
{
  if (aria_input_event.type == SDL_MOUSEBUTTONUP)
    {
      int _what = aria_input_event.wheel.y == 1 ? SDL_BUTTON_WHEEL_UP : SDL_BUTTON_WHEEL_DOWN;
      return ar_new_number(S, _what);
    }
  return NULL;
}


ar_Value* ar_mouse_mousemoved(ar_State* S, ar_Value* args, ar_Value* env)
{
  if (aria_input_event.type == SDL_MOUSEMOTION)
    return ar_new_pair(S, ar_new_number(S, aria_input_event.motion.x),
                       ar_new_number(S, aria_input_event.motion.y));
  return NULL;
}


void ar_input_register(ar_State* S)
{
  struct { const char* name; ar_Prim fn; } prims[] =
  {
  {"love:mouse-pressed", ar_mouse_mousepressed},
  {"love:mouse-released", ar_mouse_mousereleased},
  {"love:mouse-wheel", ar_mouse_mousewheel},
  {"love:mouse-moved", ar_mouse_mousemoved},
  {"love:keyboard-pressed", ar_keyboard_keypressed},
  {"love:keyboard-released", ar_keyboard_keyreleased},
  {NULL, NULL}
};
  for (int i = 0; prims[i].name; i++)
    ar_bind_global(S, prims[i].name, ar_new_prim(S, prims[i].fn));
}


