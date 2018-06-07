/*
#   clove
#
#   Copyright (C) 2016-2017 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/
#include <stdlib.h>
#include <string.h>

#include "joystick.h"

static struct {
    lua_State* luaState;
    int count;
    int joystickDataMT;
} moduleData;

static int convertToButton(const char* v) {
    if (strncmp("a", v, 1) == 0)
        return SDL_CONTROLLER_BUTTON_A;
    if (strncmp("b", v, 1) == 0)
        return SDL_CONTROLLER_BUTTON_B;
    if (strncmp("x", v, 1) == 0)
        return SDL_CONTROLLER_BUTTON_X;
    if (strncmp("y", v, 1) == 0)
        return SDL_CONTROLLER_BUTTON_Y;
    if (strncmp("back", v, 4) == 0)
        return SDL_CONTROLLER_BUTTON_BACK;
    if (strncmp("guide", v, 5) == 0)
        return SDL_CONTROLLER_BUTTON_GUIDE;
    if (strncmp("start", v, 5) == 0)
        return SDL_CONTROLLER_BUTTON_START;
    if (strncmp("leftstick", v, 9) == 0)
        return SDL_CONTROLLER_BUTTON_LEFTSTICK;
    if (strncmp("rightstick", v, 10) == 0)
        return SDL_CONTROLLER_BUTTON_RIGHTSTICK;
    if (strncmp("leftshoulder", v, 12) == 0)
        return SDL_CONTROLLER_BUTTON_LEFTSHOULDER;
    if (strncmp("rightshoulder", v, 13) == 0)
        return SDL_CONTROLLER_BUTTON_RIGHTSHOULDER;
    if (strncmp("dpup", v, 4) == 0)
        return SDL_CONTROLLER_BUTTON_DPAD_UP;
    if (strncmp("dpdown", v, 6) == 0)
        return SDL_CONTROLLER_BUTTON_DPAD_DOWN;
    if (strncmp("dpleft", v, 6) == 0)
        return SDL_CONTROLLER_BUTTON_DPAD_LEFT;
    if (strncmp("dpright", v, 7) == 0)
        return SDL_CONTROLLER_BUTTON_DPAD_RIGHT;
    return 0;
}

static const char* convertFromButton(int v) {
    if (v == SDL_CONTROLLER_BUTTON_A)
        return "a";
     if (v == SDL_CONTROLLER_BUTTON_B)
        return "b";
     if (v == SDL_CONTROLLER_BUTTON_X)
        return "x";
     if (v == SDL_CONTROLLER_BUTTON_Y)
        return "y";
     if (v == SDL_CONTROLLER_BUTTON_BACK)
        return "back";
     if (v == SDL_CONTROLLER_BUTTON_GUIDE)
        return "guide";
     if (v == SDL_CONTROLLER_BUTTON_START)
        return "start";
     if (v == SDL_CONTROLLER_BUTTON_LEFTSTICK)
        return "leftstick";
     if (v == SDL_CONTROLLER_BUTTON_RIGHTSTICK)
        return "rightstick";
     if (v == SDL_CONTROLLER_BUTTON_LEFTSHOULDER)
        return "leftshoulder";
     if (v == SDL_CONTROLLER_BUTTON_RIGHTSHOULDER)
        return "rightshoulder";
     if (v == SDL_CONTROLLER_BUTTON_DPAD_UP)
        return "dpup";
     if (v == SDL_CONTROLLER_BUTTON_DPAD_DOWN)
        return "dpdown";
     if (v == SDL_CONTROLLER_BUTTON_DPAD_LEFT)
        return "dpleft";
     if (v == SDL_CONTROLLER_BUTTON_DPAD_RIGHT)
        return "dpright"; 
    return "";
}

static int l_joystick_isDown(lua_State* state) {
    int id = lua_tointeger(state, 1);
    int top = lua_gettop(state);
    bool any = false;
    for (int i = 1; i < top; i++) {
        int button = convertToButton(lua_tostring(state, i + 1));
        joystick_Joystick* js = joystick_get(id);
        any = joystick_isDown(js, button);
        if (any)
            break;
    }
    lua_pushboolean(state, any);
    return 1;
}

void l_joystick_pressed(int id, int button) { 
  lua_getglobal(moduleData.luaState, "love");
  lua_pushstring(moduleData.luaState, "joystickpressed");
  lua_rawget(moduleData.luaState, -2);
  lua_pushinteger(moduleData.luaState, id);
  lua_pushstring(moduleData.luaState, convertFromButton(button));
  lua_call(moduleData.luaState, 2, 0);
}

void l_joystick_released(int id, int button) { 
  lua_getglobal(moduleData.luaState, "love");
  lua_pushstring(moduleData.luaState, "joystickreleased");
  lua_rawget(moduleData.luaState, -2);
  lua_pushinteger(moduleData.luaState, id);
  lua_pushstring(moduleData.luaState, convertFromButton(button));
  lua_call(moduleData.luaState, 2, 0);
}

static int l_joystick_getNumAxes(lua_State* state) {
    int id = lua_tointeger(state, 1);
    joystick_Joystick* js = joystick_get(id);
    lua_pushnumber(state, joystick_getNumAxes(js));
    return 1;
}

static int l_joystick_getAxis(lua_State* state) {
    int id = lua_tointeger(state, 1);
    int axis = 0;
    int size = 0;
    joystick_Joystick* js = joystick_get(id);

    axis = lua_tointeger(state, 2);
    lua_pushnumber(state, joystick_getAxis(js, axis));

    return 1;
}

static int l_joystick_getJoystickCount(lua_State* state) {
    lua_pushnumber(state, joystick_getCount());
    return 1;
}

static int l_joystick_getHatCount(lua_State *state) {
    int id = lua_tointeger(state, 1);
    joystick_Joystick *js = joystick_get(id);
    lua_pushinteger(state, joystick_getHatCount(js));
    return 1;
}

static int fromCharHatToInt(const char* v) {
    if (strncmp(v, "c", 1) == 0)
        return SDL_HAT_CENTERED;
    if (strncmp(v, "d", 1) == 0)
        return SDL_HAT_DOWN;
    if (strncmp(v, "l", 1) == 0)
        return SDL_HAT_LEFT;
    if (strncmp(v, "ld", 2) == 0)
        return SDL_HAT_LEFTDOWN;
    if (strncmp(v, "lu", 2) == 0)
        return SDL_HAT_LEFTUP;
    if (strncmp(v, "r", 1) == 0)
        return SDL_HAT_RIGHT;
    if (strncmp(v, "rd", 2) == 0)
        return SDL_HAT_RIGHTDOWN;
    if (strncmp(v, "ru", 2) == 0)
        return SDL_HAT_RIGHTUP;
    if (strncmp(v, "u", 1) == 0)
        return SDL_HAT_UP;
    
    return SDL_HAT_CENTERED;
}

static int l_joystick_getHat(lua_State *state) {
  int id = lua_tonumber(state, 1);
  joystick_Joystick* js = joystick_get(id);
  int hat = fromCharHatToInt(lua_tostring(state, 2));
  int dir = joystick_getHat(js, hat);
  lua_pushboolean(state, hat == dir);
  return 1;
}

static int l_joystick_getName(lua_State* state) {
    int id = lua_tointeger(state, 1);
    joystick_Joystick* js = joystick_get(id);
    lua_pushstring(state, joystick_getName(js));
    return 1;
}

static int l_joystick_isConnected(lua_State* state) {
    int id = lua_tointeger(state, 1);
    joystick_Joystick* js = joystick_get(id);
    lua_pushboolean(state, joystick_isConnected(js));
    return 1;
}

static int l_joystick_isGamepad(lua_State* state) {
    int id = lua_tointeger(state, 1);
    joystick_Joystick* js = joystick_get(id);
    lua_pushboolean(state, joystick_isGamepad(js));
    return 1;
}

static int l_joystick_getBallsCount(lua_State* state) {
    int id = lua_tointeger(state, 1);
    joystick_Joystick* js = joystick_get(id);
    lua_pushinteger(state, joystick_getNumButtons(js));
    return 1;
}

static float returnGamepadAxis(const char* v) {
    if (strncmp(v, "leftx", 5) == 0)
        return SDL_CONTROLLER_AXIS_LEFTX;
     if (strncmp(v, "lefty", 5) == 0)
        return SDL_CONTROLLER_AXIS_LEFTY;
     if (strncmp(v, "rightx", 6) == 0)
        return SDL_CONTROLLER_AXIS_RIGHTX;
     if (strncmp(v, "righty", 6) == 0)
        return SDL_CONTROLLER_AXIS_RIGHTY;
     if (strncmp(v, "left_trigger", 12) == 0)
        return SDL_CONTROLLER_AXIS_TRIGGERLEFT;
     if (strncmp(v, "right_trigger", 13) == 0)
        return SDL_CONTROLLER_AXIS_TRIGGERRIGHT;
    return SDL_CONTROLLER_AXIS_INVALID;
}

static int l_joystick_getGamepadAxis(lua_State *state) {
  int id = lua_tointeger(state, 1);
  float get_axis = returnGamepadAxis(lua_tostring(state, 2));
  joystick_Joystick* js = joystick_get(id);
  
  float return_axis = joystick_getGamepadAxis(js, get_axis);
  
  lua_pushboolean(state, get_axis == return_axis);
  return 1;
}

static luaL_Reg const regFuncs[] = {
    {"getGamepadAxis", l_joystick_getGamepadAxis},
    {"getBallsCount", l_joystick_getBallsCount},
    {"getHatCount", l_joystick_getHatCount},
    {"getHat", l_joystick_getHat},
    {"getAxisCount", l_joystick_getNumAxes},
    {"getAxis", l_joystick_getAxis},
    {"getName", l_joystick_getName},
    {"isConnected", l_joystick_isConnected},
    {"isGamepad", l_joystick_isGamepad},
    {"isDown", l_joystick_isDown},
    {"getCount",  l_joystick_getJoystickCount},
    {NULL, NULL}
};

void l_joystick_register(lua_State* state) {
    moduleData.luaState = state;
    l_tools_registerModule(state, "joystick", regFuncs);
}

