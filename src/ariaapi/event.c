/*
#   clove
#
#   Copyright (C) 2018 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/
#include "event.h"
#include "../3rdparty/aria/aria.h"

static ar_Value* ar_event_quit(ar_State* S, ar_Value* args, ar_Value* env)
{
  UNUSED(args);
  UNUSED(env);
  ar_running = false;
  return ar_running == false ? S->t : NULL;
}

void ar_event_register(ar_State* S)
{
  struct { const char* name; ar_Prim fn; } prims[] =
  {
  {"love:quit", ar_event_quit},
  {NULL, NULL}
};
  for (int i = 0; prims[i].name; i++)
    ar_bind_global(S, prims[i].name, ar_new_prim(S, prims[i].fn));
}
