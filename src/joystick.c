/*
#   clove
#
#   Copyright (C) 2016-2018 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/

#ifdef USE_LUA
#include "include/joystick.h"
#include "luaapi/joystick.h"

static struct {
    joystick_Joystick* list;
    int joystick_count;
} moduleData;

joystick_Joystick* joystick_get(SDL_JoystickID id) {
    for (int i = 0; i < moduleData.joystick_count; i++) {
        joystick_Joystick* js = &moduleData.list[i];
        if (js->id == id)
            return js;
    }
    printf("CLove error, returning null joystick instance \n");
    return 0;
}

static joystick_Joystick* openJoystick(int index) {
    moduleData.list->joystick = SDL_JoystickOpen(index);

    if (moduleData.list->joystick == NULL)
        printf("Joystick error: %s \n", SDL_GetError());

    moduleData.list->id = SDL_JoystickInstanceID(moduleData.list->joystick);

    if (SDL_IsGameController(index))
        moduleData.list->controller = SDL_GameControllerOpen(index);
    else
        moduleData.list->controller = 0;

    return moduleData.list;
}

void joystick_init() {
    moduleData.list = realloc(moduleData.list, sizeof(joystick_Joystick) * 1);
    if(SDL_InitSubSystem(SDL_INIT_JOYSTICK) != 0)
        printf("Joystick error %s \n", SDL_GetError());

    if(SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER) != 0)
        printf("Joystick error %s \n", SDL_GetError());

    moduleData.joystick_count = SDL_NumJoysticks();
}

void joystick_added(int id) {
    joystick_Joystick* js = openJoystick(id);
}

// Close all devices when clove quits
void joystick_close() {
    for (int i = 0; i < moduleData.joystick_count; i++) {
        joystick_Joystick* js = joystick_get(i);
        if (SDL_JoystickGetAttached(js->joystick))
            SDL_JoystickClose(js->joystick);
        free(js);
    }
}
// Close a certain device when it has been disconected from machine
void joystick_remove(int id) {
    joystick_Joystick* js = joystick_get(id);
    SDL_JoystickClose(js->joystick);
}

float joystick_getAxis(joystick_Joystick* joystick, int axis) {
    int16_t val = SDL_JoystickGetAxis(joystick->joystick, axis);
    return val / 32767.0f;
}

bool joystick_isDown(joystick_Joystick* joystick, int button) {
    return SDL_JoystickGetButton(joystick->joystick, button);
}

void joystick_buttonDown(int id, int button, int state) {
    l_joystick_pressed(id, button);
}

void joystick_buttonUp(int id, int button, int state) {
    l_joystick_released(id, button);
}

int joystick_getCount() {
    return moduleData.joystick_count;
}

int joystick_getNumAxes(joystick_Joystick* joystick) {
    return SDL_JoystickNumAxes(joystick->joystick);
}

int joystick_getNumButtons(joystick_Joystick* joystick) {
    return SDL_JoystickNumButtons(joystick->joystick);
}

int joystick_getNumBalls(joystick_Joystick* joystick) {
    return SDL_JoystickNumBalls(joystick->joystick);
}

const char* joystick_getName(joystick_Joystick* joystick) {
    return SDL_JoystickNameForIndex(joystick->id);
}

bool joystick_isConnected(joystick_Joystick* joystick) {
    return SDL_JoystickGetAttached(joystick->joystick);
}

bool joystick_isGamepad(joystick_Joystick* joystick) {
    return joystick->controller != 0;
}

int joystick_getHatCount(joystick_Joystick* joystick) {
    return SDL_JoystickNumHats(joystick->joystick);
}

int joystick_getHat(joystick_Joystick* joystick, int hat) {
    return SDL_JoystickGetHat(joystick->joystick, hat);
}

float joystick_getGamepadAxis(joystick_Joystick* joystick, int axis) {
    return SDL_GameControllerGetAxis(joystick->controller, (SDL_GameControllerAxis) axis) / 32767.0f;
}

#endif //USE_LUA
