/*
#   clove
#
#   Copyright (C) 2016-2017 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/
#ifdef EMSCRIPTEN
 #include <emscripten.h>
#endif

#include "../3rdparty/SDL2/include/SDL.h"

#include "timer.h"

static float const FpsUpdateTimeout = 1.0f;

static struct {
  float averageDeltaTime;
  float lastFpsUpdate;
  float currentTime;
  float deltaTime;
  float fps;
  int frames;
} moduleData;

void timer_init() {
  float now = timer_getTime();
  moduleData.averageDeltaTime = 0.0;
  moduleData.lastFpsUpdate = now;
  moduleData.currentTime = now;
  moduleData.deltaTime = 0.0f;
  moduleData.fps = 0.0f;
  moduleData.frames = 0;
}

float timer_getTime(void) {
#ifdef EMSCRIPTEN
  return emscripten_get_now() / 1000.0f;
#else
  return SDL_GetTicks() / 1000.0f;
#endif
}

void timer_step(void) {
  ++moduleData.frames;

  float last = moduleData.currentTime;
  moduleData.currentTime = timer_getTime();
  moduleData.deltaTime = moduleData.currentTime - last;

  float timeSinceLastUpdate = moduleData.currentTime - moduleData.lastFpsUpdate;
  if(timeSinceLastUpdate > FpsUpdateTimeout) {
    moduleData.averageDeltaTime = timeSinceLastUpdate / moduleData.frames;
    moduleData.fps = moduleData.frames / timeSinceLastUpdate;
    moduleData.lastFpsUpdate = moduleData.currentTime;
    moduleData.frames = 0;
  }
}

void timer_sleep(short ms) {
    SDL_Delay(ms * 1000.0f);
}

float timer_getFPS(void) {
  return moduleData.fps;
}

float timer_getDelta(void) {
  return moduleData.deltaTime;
}

float timer_getAverageDelta(void) {
  return moduleData.averageDeltaTime;
}


