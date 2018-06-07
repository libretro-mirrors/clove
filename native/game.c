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

#include "../src/graphics/graphics.h"
#include "../src/graphics/geometry.h"
#include "../src/graphics/matrixstack.h"
#include "../src/keyboard.h"
#include "../src/luaapi/event.h"

#include "../src/3rdparty/Chipmunk2D/include/chipmunk/chipmunk.h"

static cpSpace* space;


#define WIDTH 232
#define HEIGHT 32

int width2 = 150;

cpBody* body;

cpBody*
add_domino(cpSpace *space)
{
	cpFloat mass = 2.0f;
    cpFloat moment = cpMomentForBox(mass, width2, HEIGHT);

	cpBody *body = cpSpaceAddBody(space, cpBodyNew(mass, moment));
    body->p = cpv(200, 200);

    cpShape *shape = cpBoxShapeNew(body, width2, HEIGHT);
	cpSpaceAddShape(space, shape);
	shape->e = 0.4f; shape->u = 0.6f;

    return body;
}


static cpSpace* init(void)
{
    space = cpSpaceNew();
    space->iterations = 30;
    space->gravity = cpv(0, -100);

    body = add_domino(space);


    cpShape *shape = cpSpaceAddShape(space, cpBoxShapeNew(space->staticBody, WIDTH, HEIGHT));
	shape->e = 1.0f; shape->u = 1.0f;


    return space;
}

void game_load() {
   // graphics_setTitle("Clove in native");
    space = init();
}

void game_update(float delta) {
    if (keyboard_ispressed(SDLK_d))
    {
    }
    if (keyboard_ispressed(SDLK_a))
    {
    }
    cpSpaceStep(space, delta);
}

void game_draw() {
    graphics_setBackgroundColor(.8f, .6f, .5f, 1);
    graphics_geometry_rectangle(true, body->p.x, body->p.y, width2, HEIGHT, body->a, 1, 1, 0, 0);
    graphics_geometry_rectangle(true, 0, 40, WIDTH, HEIGHT, 0, 1, 1, 0, 0);
    //graphics_geometry_rectangle(true, ground->body->p.x, ground->body->p.y, 800, 32, 0, 1, 1, 0, 0);
}

void game_quit()
{
    cpSpaceFree(space);
}

